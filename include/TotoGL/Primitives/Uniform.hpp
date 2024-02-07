#pragma once

#include <GL/glew.h>

#include "TotoGL/GPUPointer/Shader.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <variant>

namespace TotoGL {

using UniformVariant = std::variant<
    int, float, double, bool,
    glm::vec2, glm::vec3, glm::vec4,
    glm::ivec2, glm::ivec3, glm::ivec4,
    glm::bvec2, glm::bvec3, glm::bvec4,
    glm::dvec2, glm::dvec3, glm::dvec4,
    glm::mat2, glm::mat3, glm::mat4,
    glm::mat2x3, glm::mat2x4,
    glm::mat3x2, glm::mat3x4,
    glm::mat4x2, glm::mat4x3>;

class Uniform {
public:
    Uniform(const ProgramId& program, const std::string& name)
        : _location(glGetUniformLocation(program.id(), name.c_str())) { }

    void operator()(int value) {
        glUniform1i(_location, value);
    }

    void operator()(float value) {
        glUniform1f(_location, value);
    }

    void operator()(double value) {
        glUniform1d(_location, value);
    }

    void operator()(bool value) {
        glUniform1i(_location, static_cast<GLint>(value));
    }

    void operator()(const glm::vec2& value) {
        glUniform2fv(_location, 1, glm::value_ptr(value));
    }

    void operator()(const glm::vec3& value) {
        glUniform3fv(_location, 1, glm::value_ptr(value));
    }

    void operator()(const glm::vec4& value) {
        glUniform4fv(_location, 1, glm::value_ptr(value));
    }

    void operator()(const glm::ivec2& value) {
        glUniform2iv(_location, 1, glm::value_ptr(value));
    }

    void operator()(const glm::ivec3& value) {
        glUniform3iv(_location, 1, glm::value_ptr(value));
    }

    void operator()(const glm::ivec4& value) {
        glUniform4iv(_location, 1, glm::value_ptr(value));
    }

    void operator()(const glm::bvec2& value) {
        glUniform2iv(_location, 1, glm::value_ptr(glm::ivec2(value)));
    }

    void operator()(const glm::bvec3& value) {
        glUniform3iv(_location, 1, glm::value_ptr(glm::ivec3(value)));
    }

    void operator()(const glm::bvec4& value) {
        glUniform4iv(_location, 1, glm::value_ptr(glm::ivec4(value)));
    }

    void operator()(const glm::dvec2& value) {
        glUniform2dv(_location, 1, glm::value_ptr(value));
    }

    void operator()(const glm::dvec3& value) {
        glUniform3dv(_location, 1, glm::value_ptr(value));
    }

    void operator()(const glm::dvec4& value) {
        glUniform4dv(_location, 1, glm::value_ptr(value));
    }

    void operator()(const glm::mat2& value) {
        glUniformMatrix2fv(_location, 1, GL_FALSE, glm::value_ptr(value));
    }

    void operator()(const glm::mat3& value) {
        glUniformMatrix3fv(_location, 1, GL_FALSE, glm::value_ptr(value));
    }

    void operator()(const glm::mat4& value) {
        glUniformMatrix4fv(_location, 1, GL_FALSE, glm::value_ptr(value));
    }

    void operator()(const glm::mat2x3& value) {
        glUniformMatrix2x3fv(_location, 1, GL_FALSE, glm::value_ptr(value));
    }

    void operator()(const glm::mat2x4& value) {
        glUniformMatrix2x4fv(_location, 1, GL_FALSE, glm::value_ptr(value));
    }

    void operator()(const glm::mat3x2& value) {
        glUniformMatrix3x2fv(_location, 1, GL_FALSE, glm::value_ptr(value));
    }

    void operator()(const glm::mat3x4& value) {
        glUniformMatrix3x4fv(_location, 1, GL_FALSE, glm::value_ptr(value));
    }

    void operator()(const glm::mat4x2& value) {
        glUniformMatrix4x2fv(_location, 1, GL_FALSE, glm::value_ptr(value));
    }

    void operator()(const glm::mat4x3& value) {
        glUniformMatrix4x3fv(_location, 1, GL_FALSE, glm::value_ptr(value));
    }

private:
    GLuint _location;
};

} // namespace TotoGL
