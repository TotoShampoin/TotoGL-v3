#pragma once

#include "Mesh.hpp"
#include "ShaderMaterial.hpp"
#include "TotoGL/Misc/Factory.hpp"
#include "TotoGL/Primitives/Transform.hpp"

namespace TotoGL {

class RenderObject {
public:
    RenderObject(const MeshFactory::ObjectInstanceId& mesh, const ShaderMaterialFactory::ObjectInstanceId& material)
        : _mesh(mesh)
        , _material(material) { }

    void draw() {
        auto& material = ShaderMaterialFactory::get(_material);
        auto& mesh = MeshFactory::get(_mesh);

        material.use();
        mesh.draw();
        material.unuse();
    }
    static void unbind() {
        ShaderProgram::unuse();
        Mesh::unbind();
    }

    RenderObject& translate(const glm::vec3& translation) {
        _transform.translate(translation);
        return *this;
    }
    RenderObject& scale(const glm::vec3& factor) {
        _transform.scale(factor);
        return *this;
    }
    RenderObject& rotate(const float& angle, const glm::vec3& axis) {
        _transform.rotate(angle, axis);
        return *this;
    }
    void lookAt(const glm::vec3& target, const glm::vec3& up = { 0, 1, 0 }) {
        _transform.lookAt(target, up);
    }

    Transform& transformation() { return _transform; }
    glm::mat4 transformMatrix() const { return _transform.matrix(); }
    glm::vec3& position() { return _transform.translation(); }
    glm::vec3& rotation() { return _transform.rotation(); }
    glm::vec3& scaling() { return _transform.scaling(); }

    Mesh& mesh() { return MeshFactory::get(_mesh); }
    ShaderMaterial& material() { return ShaderMaterialFactory::get(_material); }

private:
    MeshFactory::ObjectInstanceId _mesh;
    ShaderMaterialFactory::ObjectInstanceId _material;
    Transform _transform;
};

using RenderObjectFactory = Factory<RenderObject>;
using RenderObjectInstanceId = Factory<RenderObject>::ObjectInstanceId;

} // namespace TotoGL
