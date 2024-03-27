#include "TotoGL/RenderObject/MaterialObject.hpp"
#include "TotoGL/Primitives/Shader.hpp"
#include "_embeds/shaders.hpp"

namespace TotoGL {

MaterialObject::MaterialObject(
    std::vector<MeshInstanceId>&& meshes,
    std::vector<MaterialData>&& materials,
    std::vector<uint>&& material_indices)
    : _meshes(std::move(meshes))
    , _materials(std::move(materials))
    , _material_indices(std::move(material_indices)) {
    static auto PHONG_SHADER = ShaderMaterialFactory::create(ShaderMaterial(
        VertexShader(vertex_shader),
        FragmentShader(prong_shader)));

    _shader_material = PHONG_SHADER;
}

RenderObject MaterialObject::get(const size_t& index) const {
    auto object = RenderObject(_meshes[index], _shader_material);
    auto& material = _materials[_material_indices[index]];
    _shader_material->uniform("u_ambient", material.ambient);
    _shader_material->uniform("u_diffuse", material.diffuse);
    _shader_material->uniform("u_specular", material.specular);
    _shader_material->uniform("u_emissive", material.emissive);
    _shader_material->uniform("u_shininess", material.shininess);
    if (material.ambient_texture) {
        _shader_material->uniform("u_ambient_texture", material.ambient_texture.value());
        _shader_material->uniform("u_use_ambient_texture", true);
    } else {
        _shader_material->uniform("u_use_ambient_texture", false);
    }
    if (material.diffuse_texture) {
        _shader_material->uniform("u_diffuse_texture", material.diffuse_texture.value());
        _shader_material->uniform("u_use_diffuse_texture", true);
    } else {
        _shader_material->uniform("u_use_diffuse_texture", false);
    }
    if (material.specular_texture) {
        _shader_material->uniform("u_specular_texture", material.specular_texture.value());
        _shader_material->uniform("u_use_specular_texture", true);
    } else {
        _shader_material->uniform("u_use_specular_texture", false);
    }
    if (material.emissive_texture) {
        _shader_material->uniform("u_emissive_texture", material.emissive_texture.value());
        _shader_material->uniform("u_use_emissive_texture", true);
    } else {
        _shader_material->uniform("u_use_emissive_texture", false);
    }
    object.transformation() = _transform;
    return object;
}

MaterialObject& MaterialObject::translate(const glm::vec3& translation) {
    _transform.translate(translation);
    return *this;
}

MaterialObject& MaterialObject::scale(const glm::vec3& factor) {
    _transform.scale(factor);
    return *this;
}

MaterialObject& MaterialObject::rotate(const float& angle, const glm::vec3& axis) {
    _transform.rotate(angle, axis);
    return *this;
}

void MaterialObject::lookAt(const glm::vec3& target, const glm::vec3& up) {
    _transform.lookAt(target, up);
}

}; // namespace TotoGL
