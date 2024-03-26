#include "TotoGL/RenderObject/RenderObject.hpp"

#include "TotoGL/Misc/Factory.hpp"
#include "TotoGL/Primitives/Transform.hpp"
#include "TotoGL/RenderObject/Mesh.hpp"
#include "TotoGL/RenderObject/ShaderMaterial.hpp"

namespace TotoGL {

RenderObject::RenderObject(MeshInstanceId&& mesh, ShaderMaterialInstanceId&& material)
    : _mesh(std::move(mesh))
    , _material(std::move(material))
    , _owns_mesh(true)
    , _owns_material(true) { }

RenderObject::RenderObject(const MeshInstanceId& mesh, const ShaderMaterialInstanceId& material)
    : _mesh(mesh)
    , _material(material)
    , _owns_mesh(false)
    , _owns_material(false) { }

RenderObject::RenderObject(RenderObject&& other)
    : _mesh(std::move(other._mesh))
    , _material(std::move(other._material))
    , _transform(std::move(other._transform))
    , _owns_mesh(other._owns_mesh)
    , _owns_material(other._owns_material) {
    other._owns_mesh = false;
    other._owns_material = false;
}

RenderObject::~RenderObject() {
    if (_owns_mesh) {
        MeshFactory::destroy(_mesh);
    }
    if (_owns_material) {
        ShaderMaterialFactory::destroy(_material);
    }
}

void RenderObject::draw() {
    auto& material = ShaderMaterialFactory::get(_material);
    auto& mesh = MeshFactory::get(_mesh);

    material.use();
    mesh.draw();
    material.unuse();
}

void RenderObject::unbind() {
    ShaderProgram::unuse();
    Mesh::unbind();
}

RenderObject& RenderObject::translate(const glm::vec3& translation) {
    _transform.translate(translation);
    return *this;
}

RenderObject& RenderObject::scale(const glm::vec3& factor) {
    _transform.scale(factor);
    return *this;
}

RenderObject& RenderObject::rotate(const float& angle, const glm::vec3& axis) {
    _transform.rotate(angle, axis);
    return *this;
}

void RenderObject::lookAt(const glm::vec3& target, const glm::vec3& up) {
    _transform.lookAt(target, up);
}

void RenderObject::setMesh(const MeshInstanceId& mesh) {
    if (_owns_mesh) {
        MeshFactory::destroy(_mesh);
    }
    _mesh = mesh;
    _owns_mesh = false;
}

void RenderObject::setMaterial(const ShaderMaterialInstanceId& material) {
    if (_owns_material) {
        ShaderMaterialFactory::destroy(_material);
    }
    _material = material;
    _owns_material = false;
}

void RenderObject::setMesh(MeshInstanceId&& mesh) {
    if (_owns_mesh) {
        MeshFactory::destroy(_mesh);
    }
    _mesh = std::move(mesh);
    _owns_mesh = true;
}

void RenderObject::setMaterial(ShaderMaterialInstanceId&& material) {
    if (_owns_material) {
        ShaderMaterialFactory::destroy(_material);
    }
    _material = std::move(material);
    _owns_material = true;
}

} // namespace TotoGL
