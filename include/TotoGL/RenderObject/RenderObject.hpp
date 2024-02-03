#pragma once

#include "Material.hpp"
#include "Mesh.hpp"
#include "TotoGL/Misc/Factory.hpp"

namespace TotoGL {

class RenderObject {
public:
    RenderObject(const MeshFactory::ObjectInstanceId& mesh, const MaterialFactory::ObjectInstanceId& material)
        : _mesh(mesh)
        , _material(material) { }

    void draw() {
        auto& material = MaterialFactory::get(_material);
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
    MaterialFactory::ObjectInstanceId _material;
};

using RenderObjectFactory = Factory<RenderObject>;

} // namespace TotoGL
