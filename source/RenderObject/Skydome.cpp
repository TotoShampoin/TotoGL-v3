#include "TotoGL/RenderObject/Skydome.hpp"

#include "TotoGL/Primitives/Shader.hpp"
#include "TotoGL/RenderObject/RenderObject.hpp"

namespace TotoGL {

// TODO: Put those to actual shader files that CMake will embed in the binary.
constexpr auto skydome_vertex_shader = R"glsl(#version 460
layout(location = 0) in vec3 a_position;out vec3 v_position;void main() { vec4 position = vec4(a_position, 1.); v_position = position.xyz; gl_Position = position;})glsl";
constexpr auto skydome_fragment_shader = R"glsl(#version 460
#define PI 3.14159265359
in vec3 v_position;out vec4 f_frag_color;uniform mat4 u_projection;uniform mat4 u_modelview;uniform sampler2D u_texture;vec2 directionToUV(vec3 direction) { float u = 0.5 + atan(direction.z, direction.x) / (2.0 * PI); float v = 0.5 - asin(direction.y) / PI; return vec2(u, v);}void main() { vec4 clip_pos = vec4(v_position, 1.0); vec4 eye_pos = inverse(u_projection) * clip_pos; eye_pos = vec4(eye_pos.xy, -1.0, 0.0); vec4 world_pos = inverse(u_modelview) * eye_pos; vec3 ray = normalize(world_pos.xyz); vec2 uv = directionToUV(ray); vec3 color = texture(u_texture, uv).rgb; f_frag_color = vec4(color, 1.0);})glsl";

Skydome::Skydome() {
    auto skydome_mesh_id = MeshFactory::create(Mesh::quad(2, 2));
    auto skydome_material_id = ShaderMaterialFactory::create(
        ShaderMaterial(
            VertexShader(skydome_vertex_shader),
            FragmentShader(skydome_fragment_shader)));

    _object = RenderObjectFactory::create(
        RenderObject(skydome_mesh_id, skydome_material_id));
}

Skydome::Skydome(Texture& texture)
    : Skydome() {
    setTexture(texture);
}

Skydome::Skydome(Skydome&& other)
    : _object(other._object)
    , _is_owner(other._is_owner) {
    other._is_owner = false;
}

Skydome::~Skydome() {
    if (_is_owner) {
        RenderObjectFactory::destroy(_object);
    }
}

void Skydome::setTexture(Texture& texture) {
    auto& material = object().material();
    material.uniform("u_texture", texture);
}

} // namespace TotoGL
