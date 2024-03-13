#pragma once

#include "TotoGL/RenderObject/Mesh.hpp"
#include <fstream>

namespace TotoGL {

Mesh loadWavefrontObj(std::ifstream&& file);

} // namespace TotoGL
