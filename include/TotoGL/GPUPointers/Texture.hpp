#pragma once

#include <GL/glew.h> // MUST STAY ON TOP

#include "./GPUPointers.hpp"

namespace TotoGL {

using Texture = GPUPointers<
    []() { GLuint b; glGenTextures(1, &b); return b; },
    [](GLuint& b) { glDeleteTextures(1, &b); }>;

}
