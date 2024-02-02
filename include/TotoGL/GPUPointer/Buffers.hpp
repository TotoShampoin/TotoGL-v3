#pragma once

#include <GL/glew.h> // MUST STAY ON TOP

#include "./GPUPointer.hpp"

namespace TotoGL {

using VboId = GPUPointer<
    []() { GLuint b; glCreateBuffers(1, &b); return b; },
    [](GLuint& b) { glDeleteBuffers(1, &b); }>;

using VaoId = GPUPointer<
    []() { GLuint b; glCreateVertexArrays(1, &b); return b; },
    [](GLuint& b) { glDeleteVertexArrays(1, &b); }>;

using IboId = GPUPointer<
    []() { GLuint b; glCreateBuffers(1, &b); return b; },
    [](GLuint& b) { glDeleteBuffers(1, &b); }>;

} // namespace TotoGL
