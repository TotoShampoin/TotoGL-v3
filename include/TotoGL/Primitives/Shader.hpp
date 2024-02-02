#pragma once

#include "TotoGL/GPUPointer/Shader.hpp"
#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <variant>

namespace TotoGL {

template <ShaderType type>
class Shader {
public:
    using ShaderType::COMPUTE;
    using ShaderType::FRAGMENT;
    using ShaderType::VERTEX;
    Shader() {
        if constexpr (type == VERTEX) {
            _shader = VertexShaderId();
        } else if constexpr (type == FRAGMENT) {
            _shader = FragmentShaderId();
        } else if constexpr (type == COMPUTE) {
            _shader = ComputeShaderId();
        }
    }
    Shader(std::ifstream&& file)
        : Shader() {
        loadAndCompile(std::move(file));
    }

    bool loadAndCompile(std::ifstream&& file) {
        load(std::move(file));
        return compile();
    }

    void load(std::ifstream&& file) {
        load(std::string(
            std::istreambuf_iterator<char>(file),
            std::istreambuf_iterator<char>()));
    }

    void load(const std::string& src) {
        auto cstr = src.c_str();
        std::visit([&](auto&& shaderId) {
            glShaderSource(shaderId.id(), 1, &cstr, NULL);
        },
            _shader);
    }

    bool compile() {
        int success;
        std::visit([&](auto&& shaderId) {
            glCompileShader(shaderId.id());
            glGetShaderiv(shaderId.id(), GL_COMPILE_STATUS, &success);
            if (!success) {
                std::array<char, 1024> error;
                glGetShaderInfoLog(shaderId.id(), 1024, NULL, error.data());
                std::cerr << error.data();
            }
        },
            _shader);
        return (bool)success;
    }

private:
    ShaderId _shader;
    friend class ShaderProgram;
    friend class Material;
};

} // namespace TotoGL
