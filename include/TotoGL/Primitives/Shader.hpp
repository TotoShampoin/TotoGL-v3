#pragma once

#include "TotoGL/GPUPointer/Shader.hpp"
#include <array>
#include <fstream>
#include <iostream>
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
        load(std::move(file));
    }

    void load(std::ifstream&& file) {
        auto str = std::string(
            std::istreambuf_iterator<char>(file),
            std::istreambuf_iterator<char>());
        auto cstr = str.c_str();

        std::visit([&](auto&& shaderId) {
            glShaderSource(shaderId.id(), 1, &cstr, NULL);
            glCompileShader(shaderId.id());
            int success;
            glGetShaderiv(shaderId.id(), GL_COMPILE_STATUS, &success);
            if (!success) {
                std::array<char, 512> error;
                glGetShaderInfoLog(shaderId.id(), 512, NULL, error.data());
                std::cerr << error.data();
            }
        },
            _shader);
    }

private:
    ShaderId _shader;
    friend class ShaderProgram;
};

} // namespace TotoGL
