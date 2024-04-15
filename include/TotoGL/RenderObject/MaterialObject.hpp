#pragma once

#include "TotoGL/Misc/Factory.hpp"
#include "TotoGL/Primitives/MaterialData.hpp"
#include "TotoGL/Primitives/Transform.hpp"
#include "TotoGL/RenderObject/Mesh.hpp"
#include "TotoGL/RenderObject/RenderObject.hpp"
#include "TotoGL/RenderObject/ShaderMaterial.hpp"
#include <vector>

namespace TotoGL {

class MaterialObject {
public:
    MaterialObject(
        std::vector<MeshInstanceId>&& meshes,
        std::vector<MaterialData>&& materials,
        std::vector<uint>&& material_indices);

    MaterialObject(
        const std::vector<MeshInstanceId>& meshes,
        const std::vector<MaterialData>& materials,
        const std::vector<uint>& material_indices);

    MaterialObject(MaterialObject&& other);
    ~MaterialObject();

    ObjectInstanceId<MaterialObject> clone() const;

    RenderObject get(const size_t& index) const;
    size_t size() const { return _meshes.size(); }

    std::vector<MeshInstanceId>& meshes() { return _meshes; }
    std::vector<MaterialData>& materials() { return _materials; }
    std::vector<uint>& materialIndices() { return _material_indices; }

    const std::vector<MeshInstanceId>& meshes() const { return _meshes; }
    const std::vector<MaterialData>& materials() const { return _materials; }
    const std::vector<uint>& materialIndices() const { return _material_indices; }
    ShaderMaterialInstanceId shaderMaterial() const { return _shader_material; }

    MaterialObject& translate(const glm::vec3& translation);
    MaterialObject& scale(const glm::vec3& factor);
    MaterialObject& rotate(const float& angle, const glm::vec3& axis);
    void lookAt(const glm::vec3& target, const glm::vec3& up = { 0, 1, 0 });

    Transform& transformation() { return _transform; }
    glm::mat4 transformMatrix() const { return _transform.matrix(); }
    glm::vec3& position() { return _transform.translation(); }
    glm::vec3& rotation() { return _transform.rotation(); }
    glm::vec3& scaling() { return _transform.scaling(); }

private:
    std::vector<MeshInstanceId> _meshes;
    std::vector<MaterialData> _materials;
    std::vector<uint> _material_indices;
    ShaderMaterialInstanceId _shader_material;
    bool _owns_meshes;
    Transform _transform;

    void initShaderMaterial();
};

using MaterialObjectFactory = Factory<MaterialObject>;
using MaterialObjectInstanceId = ObjectInstanceId<MaterialObject>;

} // namespace TotoGL
