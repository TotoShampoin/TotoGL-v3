#pragma once

#include "TotoGL/GPUPointers/Shader.hpp"
#include <array>
#include <fstream>
#include <iostream>
#include <variant>

namespace TotoGL {

class Shader {
public:
    Shader(ShaderType type) {
        switch (type) {
        case ShaderType::VERTEX:
            _shader = VertexShaderId();
            break;
        case ShaderType::FRAGMENT:
            _shader = FragmentShaderId();
            break;
        case ShaderType::COMPUTE:
            _shader = ComputeShaderId();
            break;
        }
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
