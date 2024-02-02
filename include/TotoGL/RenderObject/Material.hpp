#pragma once

#include "TotoGL/GPUPointer/Shader.hpp"
#include "TotoGL/Primitives/ShaderProgram.hpp"

namespace TotoGL {

class Material {
public:
    using ShaderType::FRAGMENT;
    using ShaderType::VERTEX;
    Material(const Shader<VERTEX>& vertex, const Shader<FRAGMENT>& frament) {
        _program.attach(vertex).attach(frament).link();
    }

    void use() {
        _program.use();
    }
    static void unuse() {
        ShaderProgram::unuse();
    }

    void uniform(const std::string& name, const UniformVariant& value) {
        _program.uniform(name, value);
    }
    void uniform(const std::string& name, Texture& value) {
        _program.uniform(name, value);
    }

private:
    ShaderProgram _program;
};

} // namespace TotoGL
