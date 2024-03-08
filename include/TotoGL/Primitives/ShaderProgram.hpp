#pragma once

#include "TotoGL/GPUPointer/Shader.hpp"
#include "TotoGL/Misc/Factory.hpp"
#include "TotoGL/Primitives/Shader.hpp"
#include "TotoGL/Primitives/Texture.hpp"
#include "TotoGL/Primitives/Uniform.hpp"
#include <GL/glew.h>
#include <array>
#include <functional>
#include <map>
#include <string>
#include <variant>

namespace TotoGL {

// I would refactor, if not for those template methods

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

    template <ShaderType type>
    ShaderProgram& attach(const Shader<type>& shader) {
        std::visit([&](auto&& shader_id) {
            glAttachShader(_program.id(), shader_id.id());
        },
            shader._shader);
        return *this;
    }

    template <ShaderType type>
    ShaderProgram& detach(const Shader<type>& shader) {
        std::visit([&](auto&& shader_id) {
            glDetachShader(_program.id(), shader_id.id());
        },
            shader._shader);
        return *this;
    }

    ShaderProgram& detachAll() {
        GLsizei count;
        std::array<GLuint, 16> shaders;
        glGetAttachedShaders(_program.id(), shaders.size(), &count, shaders.data());
        for (GLsizei i = 0; i < count; i++) {
            glDetachShader(_program.id(), shaders[i]);
        }
        return *this;
    }

    void uniform(const std::string& name, const UniformVariant& value) {
        if (!_uniforms.contains(name)) {
            _uniforms.insert_or_assign(name, Uniform(_program, name));
        }
        auto& uniform = _uniforms.at(name);
        std::visit(uniform, value);
    }
    // ! Not too sure about the safety of this code
    void uniform(const std::string& name, Texture& value) {
        if (!_textures.contains(name)) {
            _textures.insert_or_assign(name, value);
        }
        int map_index = std::distance(_textures.begin(), _textures.find(name));
        uniform(name, map_index);
        glActiveTexture(GL_TEXTURE0 + map_index);
        value.bind();
    }

private:
    ProgramId _program;
    std::map<std::string, Uniform> _uniforms;
    std::map<std::string, std::reference_wrapper<Texture>> _textures;
};

using ShaderProgramFactory = Factory<ShaderProgram>;
using ShaderProgramInstanceId = Factory<ShaderProgram>::ObjectInstanceId;

} // namespace TotoGL
