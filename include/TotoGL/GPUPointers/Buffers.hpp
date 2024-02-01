#pragma once

#include <GL/glew.h> // MUST STAY ON TOP

#include "./GPUPointers.hpp"

namespace TotoGL {

using VboId = GPUPointers<
    []() { GLuint b; glCreateBuffers(1, &b); return b; },
    [](GLuint& b) { glDeleteBuffers(1, &b); }>;

using VaoId = GPUPointers<
    []() { GLuint b; glCreateVertexArrays(1, &b); return b; },
    [](GLuint& b) { glDeleteVertexArrays(1, &b); }>;

using IboId = GPUPointers<
    []() { GLuint b; glCreateBuffers(1, &b); return b; },
    [](GLuint& b) { glDeleteBuffers(1, &b); }>;

} // namespace TotoGL
