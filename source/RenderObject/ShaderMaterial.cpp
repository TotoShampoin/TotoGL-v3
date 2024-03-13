#include "TotoGL/RenderObject/ShaderMaterial.hpp"

#include "TotoGL/Primitives/Shader.hpp"
#include "TotoGL/Primitives/ShaderProgram.hpp"
#include "TotoGL/Primitives/Texture.hpp"

namespace TotoGL {

ShaderMaterial::ShaderMaterial()
    : ShaderMaterial(
        Shader<VERTEX>(),
        Shader<FRAGMENT>()) { }

ShaderMaterial::ShaderMaterial(Shader<VERTEX>&& vertex, Shader<FRAGMENT>&& frament)
    : _vertex(std::move(vertex))
    , _fragment(std::move(frament)) {
    compile();
}

void ShaderMaterial::compile() {
    _program.detachAll().attach(_vertex).attach(_fragment).link();
}

void ShaderMaterial::use() {
    _program.use();
}
void ShaderMaterial::unuse() {
    ShaderProgram::unuse();
}

void ShaderMaterial::uniform(const std::string& name, const UniformVariant& value) {
    use();
    _program.uniform(name, value);
    unuse();
}

void ShaderMaterial::uniform(const std::string& name, TextureInstanceId value) {
    _program.uniform(name, TextureFactory::get(value));
}
void ShaderMaterial::uniform(const std::string& name, Texture& value) {
    use();
    _program.uniform(name, value);
    unuse();
}

} // namespace TotoGL
