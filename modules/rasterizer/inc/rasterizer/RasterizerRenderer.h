#pragma once

#include "core/Material.h"
#include "core/MeshData.h"
#include "core/Scene.h"
#include "rasterizer/BackFaceCuller.h"
#include "rasterizer/Clipper.h"
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

        for (Shape* shape : scene.shapes()) {
            const RasterizerShaders& shaders = static_cast<const RasterizerShaders&>(shape->shaderGroup());
            MeshData shapeMesh = shaders.shapeShader().generateMesh(*shape);
            meshToGlobalSpace(shapeMesh, *shape);
            MeshData culledMesh = cullBackFaceTriangles(shapeMesh, bfCuller);
            MeshData finalMesh = clipper.clip(culledMesh);

            glm::mat4 toScreenMatrix =
                camera.viewportTransform() * camera.projectionTransform() * camera.cameraTransform();
            verticesToScreenSpace(toScreenMatrix, finalMesh);
            rasterizeMesh(finalMesh, screen);
        }
    }

private:
    void meshToGlobalSpace(MeshData& mesh, const Shape& shape);
    MeshData cullBackFaceTriangles(MeshData& mesh, const BackFaceCuller& culler);
    void verticesToScreenSpace(const glm::mat4& toScreenMatrix, MeshData& mesh);

    void rasterizeMesh(const MeshData& mesh, Screen auto& screen) {
        auto painter = screen.paintPixels();
        const auto& vertices = mesh.vertices();
        for (const auto& triangle : mesh.triangles()) {
            TriangleRasterizer::rasterize(vertices[triangle[0].vertex], vertices[triangle[1].vertex],
                                          vertices[triangle[2].vertex], painter);
        }
    }
};

static_assert(Renderer<RasterizerRenderer>, "RasterizerRenderer does not fulfil the Renderer concept.");
} // namespace cg
