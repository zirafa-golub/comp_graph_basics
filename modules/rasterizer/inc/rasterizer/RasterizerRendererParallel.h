#pragma once

#include "core/Color.h"
#include "core/Material.h"
#include "core/MeshData.h"
#include "core/Scene.h"
#include "rasterizer/BackFaceCuller.h"
#include "rasterizer/Clipper.h"
#include "rasterizer/DepthBuffer.h"
#include "rasterizer/MemoryColorBuffer.h"
#include "rasterizer/RasterizerShaders.h"
#include "rasterizer/TriangleRasterizer.h"
#include "renderer/Renderer.h"
#include "task/TaskGraph.h"
#include "task/ThreadPool.h"

#include "glm/mat4x4.hpp"

#include <algorithm>
#include <vector>

namespace cg {
class Camera;

class RasterizerRendererParallel {
public:
    void renderScene(Scene& scene, Screen auto& screen) {
        const Camera& camera = scene.camera();
        BackFaceCuller bfCuller(camera.position());
        FrustumIntersect frustumIntersect(scene.camera().frustumPoints());
        std::vector<Clipper> clippers(threadPool_.threadCount(),
                                      Clipper(frustumIntersect, FrustumIntersect::Near | FrustumIntersect::Far));
        prepareBuffers(screen.width(), screen.height());
        glm::mat4 toScreenMatrix = camera.viewportTransform() * camera.projectionTransform() * camera.cameraTransform();

        TaskSequence renderSequence;

        TaskBatch bufferClearBatch;
        for (unsigned i = 0; i < depthBuffers_.size(); ++i) {
            bufferClearBatch.addWork([this, i]() {
                depthBuffers_[i].clear();
            });
        }
        for (unsigned i = 0; i < colorBuffers_.size(); ++i) {
            bufferClearBatch.addWork([this, i]() {
                colorBuffers_[i].clear();
            });
        }
        renderSequence.addWork(std::move(bufferClearBatch));

        TaskBatch shapesBatch;
        for (Shape* shape : scene.shapes()) {
            TaskSequence perShapeSteps;
            auto tempData = std::make_unique<ShapeTempData>();

            perShapeSteps.addWork([this, tempData = tempData.get(), &bfCuller, &clippers, &toScreenMatrix, shape]() {
                auto threadIndex = ThreadPool::threadIndex();

                const RasterizerShaders& shaders = static_cast<const RasterizerShaders&>(shape->shaderGroup());
                MeshData shapeMesh = shaders.shapeShader().generateMesh(*shape);

                meshToGlobalSpace(shapeMesh, *shape);

                MeshData culledMesh = cullBackFaceTriangles(shapeMesh, bfCuller);
                MeshData clippedMesh = clippers[threadIndex].clip(culledMesh);

                tempData->globalVertices.assign_range(clippedMesh.vertices());

                tempData->invertedW = verticesToScreenSpace(toScreenMatrix, clippedMesh);
                tempData->screenMesh = std::move(clippedMesh);
            });

            perShapeSteps.addDynamicWork([this, shape, tempData = tempData.get(), &scene, &screen]() {
                TaskBatch triangleRasterBatch;
                auto triangles = tempData->screenMesh.triangles();
                for (auto i = 0; i < triangles.size(); i += maxTrianglesPerTask) {
                    unsigned trianglesPerTask =
                        std::min(static_cast<unsigned>(triangles.size() - i), maxTrianglesPerTask);
                    auto taskTriangles = std::span(triangles.begin() + i, trianglesPerTask);

                    triangleRasterBatch.addWork([this, shape, tempData, taskTriangles, &scene, &screen]() {
                        auto threadIndex = ThreadPool::threadIndex();
                        if (threadIndex == 0) {
                            FragmentPainter painter(screen.paintPixels(), depthBuffers_[threadIndex], scene, *shape);
                            rasterizeTriangles(taskTriangles, tempData->screenMesh, tempData->globalVertices,
                                               tempData->invertedW, painter);
                        } else {
                            FragmentPainter painter(colorBuffers_[threadIndex - 1].paintPixels(),
                                                    depthBuffers_[threadIndex], scene, *shape);
                            rasterizeTriangles(taskTriangles, tempData->screenMesh, tempData->globalVertices,
                                               tempData->invertedW, painter);
                        }
                    });
                }
                return triangleRasterBatch;
            });

            perShapeSteps.addWork([tempData = std::move(tempData)]() {
                // This task is just to keep the data alive until all tasks using it are done
            });

            shapesBatch.addWork(std::move(perShapeSteps));
        }
        renderSequence.addWork(std::move(shapesBatch));

        TaskBatch colorCombineBatch;
        for (int row = 0; row < screen.height(); row += maxRowsPerTask) {
            unsigned rowsPerTask = std::min(static_cast<unsigned>(screen.height() - row), maxRowsPerTask);
            colorCombineBatch.addWork([this, startRow = row, rowsPerTask, &screen]() {
                combineColorBuffers(startRow, rowsPerTask, screen.paintPixels());
            });
        }
        renderSequence.addWork(std::move(colorCombineBatch));

        renderSequence.startAndWait(threadPool_);
    }

private:
    struct ShapeTempData {
        std::vector<Point> globalVertices;
        std::vector<float> invertedW;
        MeshData screenMesh;
    };

    template <PixelPainter Painter>
    class FragmentPainter {
    public:
        FragmentPainter(Painter&& painter, DepthBuffer& depthBuffer, const Scene& scene, const Shape& shape)
            : painter_(std::move(painter)), depthBuffer_(depthBuffer), scene_(scene), shape_(shape) {}
        void paintFragment(const FragmentData& frag) {
            bool shouldPaint = depthBuffer_.updateIfNearer(frag.x, frag.y, frag.z);
            if (!shouldPaint) {
                return;
            }
            glm::vec3 unitViewDir = glm::normalize(scene_.camera().position() - frag.pos3d);
            Color pixelColor;
            for (const auto& light : scene_.lights()) {
                auto lightDistance = light->distanceFrom(frag.pos3d);
                Color reflectedLight = shape_.material().reflect(frag.normal, unitViewDir, lightDistance.unitDirection);
                pixelColor += reflectedLight * light->illuminate(frag.pos3d, frag.normal);
            }
            pixelColor += scene_.ambientLight() * shape_.ambientReflectance();
            painter_.paint(frag.y, frag.x, pixelColor);
        }
        int width() { return painter_.width(); }
        int height() { return painter_.height(); }

    private:
        Painter painter_;
        DepthBuffer& depthBuffer_;
        const Scene& scene_;
        const Shape& shape_;
    };

    void prepareBuffers(int width, int height);
    void meshToGlobalSpace(MeshData& mesh, const Shape& shape);
    MeshData cullBackFaceTriangles(MeshData& mesh, const BackFaceCuller& culler);
    std::vector<float> verticesToScreenSpace(const glm::mat4& toScreenMatrix, MeshData& mesh);

    template <PixelPainter Painter>
    void rasterizeTriangles(std::span<const TriangleData> triangles, const MeshData& mesh,
                            const std::vector<Point>& globalVertices, std::vector<float> invertedW,
                            FragmentPainter<Painter>& fragPainter) {
        const auto& vertices = mesh.vertices();
        const auto& normals = mesh.vertexNormals();

        for (const auto& triangle : triangles) {
            TriangleRasterizer::rasterize(
                {vertices[triangle[0].vertex], vertices[triangle[1].vertex], vertices[triangle[2].vertex]},
                {globalVertices[triangle[0].vertex], globalVertices[triangle[1].vertex],
                 globalVertices[triangle[2].vertex]},
                {normals[triangle[0].vertexNormal], normals[triangle[1].vertexNormal],
                 normals[triangle[2].vertexNormal]},
                {invertedW[triangle[0].vertex], invertedW[triangle[1].vertex], invertedW[triangle[2].vertex]},
                fragPainter);
        }
    }

    template <PixelPainter Painter>
    void combineColorBuffers(int startRow, int rowCount, Painter painter) {
        for (int row = startRow; row < startRow + rowCount; ++row) {
            for (int col = 0; col < painter.width(); ++col) {
                int pixelIndex = row * painter.width() + col;
                int indexOfNearest = 0;
                float nearestDepth = depthBuffers_[indexOfNearest].depthAtPixel(col, row);
                for (int i = 1; i < depthBuffers_.size(); ++i) {
                    float currBufferDepth = depthBuffers_[i].depthAtPixel(col, row);
                    if (currBufferDepth > nearestDepth) {
                        nearestDepth = currBufferDepth;
                        indexOfNearest = i;
                    }
                }
                // Write something to output only if thread index of nearest wasn't 0, since that's the thread that was
                // writing to the target buffer.
                if (indexOfNearest != 0) {
                    painter.paint(row, col, colorBuffers_[indexOfNearest - 1].colorAtPixel(row, col));
                }
            }
        }
    }

    static constexpr unsigned maxTrianglesPerTask = 3;
    static constexpr unsigned maxRowsPerTask = 20;

    ThreadPool threadPool_;
    std::vector<MemoryColorBuffer> colorBuffers_;
    std::vector<DepthBuffer> depthBuffers_;
};

static_assert(Renderer<RasterizerRendererParallel>,
              "RasterizerRendererParallel does not fulfill the Renderer concept.");
} // namespace cg
