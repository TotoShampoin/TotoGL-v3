#pragma once

#include "Material.hpp"
#include "VertexObject.hpp"

namespace TotoGL {

// ! This is a temporary class, it is very unsafe.
// ! Be sure not to deallocate what you use in it.
struct RenderObject {
    VertexObject& mesh;
    Material& material;

    RenderObject(VertexObject& mesh, Material& material)
        : mesh(mesh)
        , material(material) { }

    void draw() {
        material.use();
        mesh.draw();
    }
    static void unbind() {
        ShaderProgram::unuse();
        VertexObject::unbind();
    }
};

} // namespace TotoGL
