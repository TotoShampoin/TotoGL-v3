#include "TotoGL/RenderObject/RenderObject.hpp"

#include "TotoGL/Misc/Factory.hpp"
#include "TotoGL/Primitives/Transform.hpp"
#include "TotoGL/RenderObject/Mesh.hpp"
#include "TotoGL/RenderObject/ShaderMaterial.hpp"
#include <iostream>

namespace TotoGL {

RenderObject::RenderObject(MeshInstanceId&& mesh, ShaderMaterialInstanceId&& material)
    : _mesh(std::move(mesh))
    , _material(std::move(material))
    , _is_owner(true) { }

RenderObject::RenderObject(const MeshInstanceId& mesh, const ShaderMaterialInstanceId& material)
    : _mesh(mesh)
    , _material(material)
    , _is_owner(false) { }

RenderObject::RenderObject(RenderObject&& other)
    : _mesh(std::move(other._mesh))
    , _material(std::move(other._material))
    , _transform(std::move(other._transform))
    , _is_owner(other._is_owner) {
    other._is_owner = false;
}

RenderObject::~RenderObject() {
    if (_is_owner) {
        MeshFactory::destroy(_mesh);
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

} // namespace TotoGL
