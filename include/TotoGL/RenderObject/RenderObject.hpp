#pragma once

#include "Material.hpp"
#include "Mesh.hpp"

namespace TotoGL {

class RenderObject {
public:
    RenderObject(Mesh& mesh, Material& material)
        : _mesh(mesh)
        , _material(material) { }

    void draw() {
        _material.use();
        _mesh.draw();
    }
    static void unbind() {
        ShaderProgram::unuse();
        Mesh::unbind();
    }

private:
    Mesh& _mesh;
    Material& _material;
};

} // namespace TotoGL
