#pragma once

#include "common/Error.h"
#include "core/Mesh.h"

#include <expected>
#include <filesystem>

namespace cg {
class MeshLoader {
    static std::expected<Mesh, Error> load(std::filesystem::path path);
};
} // namespace cg