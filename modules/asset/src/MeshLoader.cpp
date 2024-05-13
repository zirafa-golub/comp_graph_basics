#include "asset/MeshLoader.h"

#include "common/Error.h"

#include "tiny_obj_loader.h"

using namespace tinyobj;

namespace cg {
std::expected<Mesh, Error> MeshLoader::load(std::filesystem::path path) {
    if (!std::filesystem::is_regular_file(path) && !std::filesystem::is_symlink(path)) {
        return std::unexpected(ErrorCode::NotFound);
    }

    ObjReader reader;
    if (!reader.ParseFromFile(path.string())) {
        return std::unexpected(Error(ErrorCode::OperationFailed, reader.Error()));
    }

    const attrib_t& attributes = reader.GetAttrib();
    std::vector<Point> vertices;
    for (unsigned i = 0; i < attributes.vertices.size(); i += 3) {
        vertices.push_back(Point(attributes.vertices[i], attributes.vertices[i + 1], attributes.vertices[i + 2]));
    }

    std::vector<glm::vec3> vertexNormals;
    for (unsigned i = 0; i < attributes.normals.size(); i += 3) {
        vertexNormals.push_back(
            glm::normalize(glm::vec3(attributes.normals[i], attributes.normals[i + 1], attributes.normals[i + 2])));
    }

    const shape_t& shape = reader.GetShapes()[0];
    std::vector<TriangleData> triangleData;
    for (unsigned i = 0; i < shape.mesh.indices.size(); i += 3) {
        const index_t& index0 = shape.mesh.indices[i];
        const index_t& index1 = shape.mesh.indices[i + 1];
        const index_t& index2 = shape.mesh.indices[i + 2];
        assert(index0.vertex_index >= 0);
        assert(index1.vertex_index >= 0);
        assert(index2.vertex_index >= 0);

        triangleData.push_back(MeshData::createTriangle({index0.vertex_index, index0.normal_index},
                                                        {index1.vertex_index, index1.normal_index},
                                                        {index2.vertex_index, index2.normal_index}));
    }

    return Mesh(MeshData(std::move(vertices), std::move(vertexNormals), std::move(triangleData)));
}
} // namespace cg
