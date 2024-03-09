#include "TotoGL/RenderObject/RenderObject.hpp"

#include "TotoGL/Misc/Factory.hpp"
#include "TotoGL/Primitives/Transform.hpp"
#include "TotoGL/RenderObject/Mesh.hpp"
#include "TotoGL/RenderObject/ShaderMaterial.hpp"

namespace TotoGL {

RenderObject::RenderObject(const MeshInstanceId& mesh, const ShaderMaterialInstanceId& material)
    : _mesh(mesh)
    , _material(material) { }

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
