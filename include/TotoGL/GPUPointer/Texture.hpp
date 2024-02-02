#pragma once

#include <GL/glew.h> // MUST STAY ON TOP

#include "./GPUPointer.hpp"

namespace TotoGL {

using TextureId = GPUPointer<
    []() { GLuint b; glGenTextures(1, &b); return b; },
    [](GLuint& b) { glDeleteTextures(1, &b); }>;

}
