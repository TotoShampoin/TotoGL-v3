#pragma once

#include "TotoGL/GPUPointer/Shader.hpp"
#include "TotoGL/Misc/Factory.hpp"
#include "TotoGL/Primitives/Shader.hpp"
#include "TotoGL/Primitives/ShaderProgram.hpp"
#include "TotoGL/Primitives/Texture.hpp"

namespace TotoGL {

class ShaderMaterial {
public:
    using ShaderType::FRAGMENT;
    using ShaderType::VERTEX;
    ShaderMaterial();

    ShaderMaterial(Shader<VERTEX>&& vertex, Shader<FRAGMENT>&& frament);

    Shader<VERTEX>& vertex() { return _vertex; }
    Shader<FRAGMENT>& fragment() { return _fragment; }

    void compile();

    void use();
    static void unuse();

    void uniform(const std::string& name, const UniformVariant& value);
    void uniform(const std::string& name, TextureInstanceId value);
    void uniform(const std::string& name, Texture& value);

private:
    Shader<VERTEX> _vertex;
    Shader<FRAGMENT> _fragment;
    ShaderProgram _program;
};

using ShaderMaterialFactory = Factory<TotoGL::ShaderMaterial>;
using ShaderMaterialInstanceId = ObjectInstanceId<TotoGL::ShaderMaterial>;

} // namespace TotoGL
