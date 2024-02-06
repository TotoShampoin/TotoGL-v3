#pragma once

#include "Mesh.hpp"
#include "ShaderMaterial.hpp"
#include "TotoGL/Misc/Factory.hpp"
#include "TotoGL/Primitives/Transformation.hpp"

namespace TotoGL {

class RenderObject {
public:
    RenderObject(const MeshFactory::ObjectInstanceId& mesh, const ShaderMaterialFactory::ObjectInstanceId& material)
        : _mesh(mesh)
        , _material(material) { }

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

    Transformation& transformation() { return _transform; }
    glm::mat4 transformMatrix() const { return _transform.matrix(); }

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

private:
    MeshFactory::ObjectInstanceId _mesh;
    ShaderMaterialFactory::ObjectInstanceId _material;
    Transformation _transform;
};

using RenderObjectFactory = Factory<RenderObject>;

} // namespace TotoGL
