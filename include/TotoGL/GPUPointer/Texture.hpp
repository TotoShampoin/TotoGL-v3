#pragma once

#include <TotoGL/GL.hpp> // MUST STAY ON TOP

#include "./GPUPointer.hpp"

namespace TotoGL {

using TextureId = GPUPointer<
    []() { GLuint b; glGenTextures(1, &b); return b; },
    [](GLuint& b) { glDeleteTextures(1, &b); }>;

using FrameBufferId = GPUPointer<
    []() { GLuint b; glGenFramebuffers(1, &b); return b; },
    [](GLuint& b) { glDeleteFramebuffers(1, &b); }>;

using RenderBufferId = GPUPointer<
    []() { GLuint b; glGenRenderbuffers(1, &b); return b; },
    [](GLuint& b) { glDeleteRenderbuffers(1, &b); }>;

} // namespace TotoGL
