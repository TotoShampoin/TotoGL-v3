#pragma once

#include "TotoGL/Misc/Factory.hpp"
#include "TotoGL/Primitives/Transform.hpp"
#include "TotoGL/RenderObject/Mesh.hpp"
#include "TotoGL/RenderObject/ShaderMaterial.hpp"

namespace TotoGL {

/**
 * @brief A mesh and a material used together.
 * @warning There is no object tree as of now. If you want an object to depend on another, you have to do that manually for now.
 *
 */
class RenderObject {
public:
    RenderObject(MeshInstanceId&& mesh, ShaderMaterialInstanceId&& material);
    RenderObject(const MeshInstanceId& mesh, const ShaderMaterialInstanceId& material);
    RenderObject(RenderObject&& other);
    ~RenderObject();

    void draw();
    static void unbind();

    RenderObject& translate(const glm::vec3& translation);
    RenderObject& scale(const glm::vec3& factor);
    RenderObject& rotate(const float& angle, const glm::vec3& axis);
    void lookAt(const glm::vec3& target, const glm::vec3& up = { 0, 1, 0 });

    Transform& transformation() { return _transform; }
    glm::mat4 transformMatrix() const { return _transform.matrix(); }
    glm::vec3& position() { return _transform.translation(); }
    glm::vec3& rotation() { return _transform.rotation(); }
    glm::vec3& scaling() { return _transform.scaling(); }

    Mesh& mesh() { return MeshFactory::get(_mesh); }
    ShaderMaterial& material() { return ShaderMaterialFactory::get(_material); }

private:
    MeshInstanceId _mesh;
    ShaderMaterialInstanceId _material;
    Transform _transform;
    bool _is_owner = true;
};

using RenderObjectFactory = Factory<RenderObject>;
using RenderObjectInstanceId = ObjectInstanceId<RenderObject>;

} // namespace TotoGL
