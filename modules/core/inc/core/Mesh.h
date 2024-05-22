#pragma once

#include "core/MeshData.h"
#include "core/Shape.h"

namespace cg {

class Mesh : public Shape {
public:
    Mesh(MeshData meshData) : meshData_(std::move(meshData)) {}
    const MeshData& meshData() const override { return meshData_; }

private:
    MeshData meshData_;
};
} // namespace cg
