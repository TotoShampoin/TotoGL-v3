#pragma once

#include "TotoGL/GPUPointers/Shader.hpp"
#include "TotoGL/Primitives/Shader.hpp"
#include <variant>

namespace TotoGL {

class ShaderProgram {
public:
    ShaderProgram& link() {
        glLinkProgram(_program.id());
        return *this;
    }

    ShaderProgram& use() {
        glUseProgram(_program.id());
        return *this;
    }
    static void unuse() {
        glUseProgram(0);
    }

    ShaderProgram& attach(const Shader& shader) {
        std::visit([&](auto&& shader_id) {
            glAttachShader(_program.id(), shader_id.id());
        },
            shader._shader);
        return *this;
    }

private:
    ProgramId _program;
};

} // namespace TotoGL
