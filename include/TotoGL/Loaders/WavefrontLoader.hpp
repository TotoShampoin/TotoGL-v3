#pragma once

#include "TotoGL/RenderObject/MaterialObject.hpp"
#include "TotoGL/RenderObject/Mesh.hpp"
#include <filesystem>
#include <fstream>

namespace TotoGL {

Mesh loadWavefrontObj(std::ifstream&& file);

MaterialObject loadWavefront(const std::filesystem::path& path);

} // namespace TotoGL
