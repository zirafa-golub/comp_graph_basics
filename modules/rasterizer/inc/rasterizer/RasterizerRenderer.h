#pragma once

#include "core/Color.h"
#include "core/Material.h"
#include "core/MeshData.h"
#include "core/Scene.h"
#include "rasterizer/BackFaceCuller.h"
#include "rasterizer/Clipper.h"
#include "rasterizer/DeptBuffer.h"
#include "rasterizer/RasterizerShaders.h"
#include "rasterizer/TriangleRasterizer.h"
#include "renderer/Renderer.h"

#include "glm/mat4x4.hpp"

#include <limits>
#include <vector>

namespace cg {
class Camera;

class RasterizerRenderer {
public:
    void renderScene(Scene& scene, Screen auto& screen) {
        const Camera& camera = scene.camera();
        BackFaceCuller bfCuller(camera.position());
        FrustumIntersect frustumIntersect(scene.camera().frustumPoints());
        Clipper clipper(frustumIntersect, FrustumIntersect::Near | FrustumIntersect::Far);
        DepthBuffer depthBuffer(screen.width(), screen.height());

        for (Shape* shape : scene.shapes()) {
            const RasterizerShaders& shaders = static_cast<const RasterizerShaders&>(shape->shaderGroup());
            MeshData shapeMesh = shaders.shapeShader().generateMesh(*shape);
            meshToGlobalSpace(shapeMesh, *shape);
            MeshData culledMesh = cullBackFaceTriangles(shapeMesh, bfCuller);
            MeshData finalMesh = clipper.clip(culledMesh);
            std::vector<Point> verticesGlobal;
            verticesGlobal.reserve(finalMesh.vertices().size());
            verticesGlobal.assign_range(finalMesh.vertices());

            glm::mat4 toScreenMatrix =
                camera.viewportTransform() * camera.projectionTransform() * camera.cameraTransform();
            std::vector<float> invertedW = verticesToScreenSpace(toScreenMatrix, finalMesh);
            FragPainter fragPainter(screen.paintPixels(), depthBuffer, scene, *shape);
            rasterizeMesh(finalMesh, verticesGlobal, invertedW, fragPainter);
        }
    }

private:
    template <PixelPainter Painter>
    class FragPainter {
    public:
        FragPainter(Painter&& painter, DepthBuffer& depthBuffer, const Scene& scene, const Shape& shape)
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
            painter_.paintPixel(frag.y, frag.x, pixelColor);
        }
        int width() { return painter_.width(); }
        int height() { return painter_.height(); }

    private:
        Painter painter_;
        DepthBuffer& depthBuffer_;
        const Scene& scene_;
        const Shape& shape_;
    };

    void meshToGlobalSpace(MeshData& mesh, const Shape& shape);
    MeshData cullBackFaceTriangles(MeshData& mesh, const BackFaceCuller& culler);
    std::vector<float> verticesToScreenSpace(const glm::mat4& toScreenMatrix, MeshData& mesh);

    template <typename PixelPainter>
    void rasterizeMesh(const MeshData& mesh, const std::vector<Point>& verticesGlobal, std::vector<float> invertedW,
                       FragPainter<PixelPainter>& fragPainter) {
        const auto& vertices = mesh.vertices();
        const auto& normals = mesh.vertexNormals();

        for (const auto& triangle : mesh.triangles()) {
            TriangleRasterizer::rasterize(
                {vertices[triangle[0].vertex], vertices[triangle[1].vertex], vertices[triangle[2].vertex]},
                {verticesGlobal[triangle[0].vertex], verticesGlobal[triangle[1].vertex],
                 verticesGlobal[triangle[2].vertex]},
                {normals[triangle[0].vertexNormal], normals[triangle[1].vertexNormal],
                 normals[triangle[2].vertexNormal]},
                {invertedW[triangle[0].vertex], invertedW[triangle[1].vertex], invertedW[triangle[2].vertex]},
                fragPainter);
        }
    }
};

static_assert(Renderer<RasterizerRenderer>, "RasterizerRenderer does not fulfill the Renderer concept.");
} // namespace cg
