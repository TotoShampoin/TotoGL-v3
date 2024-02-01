#pragma once

#include "TotoGL/GPUPointers/GPUPointers.hpp"
#include <GL/glew.h>
#include <variant>

namespace TotoGL {

enum class ShaderType {
    VERTEX = GL_VERTEX_SHADER,
    FRAGMENT = GL_FRAGMENT_SHADER,
    COMPUTE = GL_COMPUTE_SHADER
};

// This won't work, you can't use a runtime parameter as template
// // template <ShaderType type>
// // using ShaderId = GPUPointers<
// //     []() { return glCreateShader(static_cast<GLenum>(type)); },
// //     [](GLuint& s) { glDeleteShader(s); }>;

using VertexShaderId = GPUPointers<
    []() { return glCreateShader(GL_VERTEX_SHADER); },
    [](GLuint& s) { glDeleteShader(s); }>;

using FragmentShaderId = GPUPointers<
    []() { return glCreateShader(GL_FRAGMENT_SHADER); },
    [](GLuint& s) { glDeleteShader(s); }>;

using ComputeShaderId = GPUPointers<
    []() { return glCreateShader(GL_COMPUTE_SHADER); },
    [](GLuint& s) { glDeleteShader(s); }>;

using ShaderId = std::variant<VertexShaderId, FragmentShaderId, ComputeShaderId>;

using ProgramId = GPUPointers<
    []() { return glCreateProgram(); },
    [](GLuint& p) { glDeleteProgram(p); }>;

} // namespace TotoGL
