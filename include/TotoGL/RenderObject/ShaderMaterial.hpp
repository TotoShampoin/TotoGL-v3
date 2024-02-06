#pragma once

#include "TotoGL/GPUPointer/Shader.hpp"
#include "TotoGL/Misc/Factory.hpp"
#include "TotoGL/Primitives/Shader.hpp"
#include "TotoGL/Primitives/ShaderProgram.hpp"

namespace TotoGL {

class ShaderMaterial {
public:
    using ShaderType::FRAGMENT;
    using ShaderType::VERTEX;
    ShaderMaterial()
        : ShaderMaterial(
            Shader<VERTEX>(),
            Shader<FRAGMENT>()) { }

    ShaderMaterial(Shader<VERTEX>&& vertex, Shader<FRAGMENT>&& frament)
        : _vertex(std::move(vertex))
        , _fragment(std::move(frament)) {
        compile();
    }

    Shader<VERTEX>& vertex() { return _vertex; }
    Shader<FRAGMENT>& fragment() { return _fragment; }

    void compile() {
        _program.detachAll().attach(_vertex).attach(_fragment).link();
    }

    void use() {
        _program.use();
    }
    static void unuse() {
        ShaderProgram::unuse();
    }

    void uniform(const std::string& name, const UniformVariant& value) {
        use();
        _program.uniform(name, value);
        unuse();
    }
    void uniform(const std::string& name, Texture& value) {
        use();
        _program.uniform(name, value);
        unuse();
    }

private:
    Shader<VERTEX> _vertex;
    Shader<FRAGMENT> _fragment;
    ShaderProgram _program;
};

using ShaderMaterialFactory = Factory<TotoGL::ShaderMaterial>;

} // namespace TotoGL
