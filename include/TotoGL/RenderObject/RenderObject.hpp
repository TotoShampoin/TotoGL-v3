#pragma once

#include "Material.hpp"
#include "Mesh.hpp"

namespace TotoGL {

// ! This is a temporary class, it is very unsafe.
// ! Be sure not to deallocate what you use in it.
struct RenderObject {
    Mesh& mesh;
    Material& material;

    RenderObject(Mesh& mesh, Material& material)
        : mesh(mesh)
        , material(material) { }

    void draw() {
        material.use();
        mesh.draw();
    }
    static void unbind() {
        ShaderProgram::unuse();
        Mesh::unbind();
    }
};

} // namespace TotoGL
