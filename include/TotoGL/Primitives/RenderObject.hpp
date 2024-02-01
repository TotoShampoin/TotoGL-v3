#pragma once

#include "ShaderProgram.hpp"
#include "VertexObject.hpp"

namespace TotoGL {

// ! This is a temporary class, it is very unsafe.
// ! Be sure not to deallocate what you use in it.
// ! Ideally, you would create only in the scope you actually need it for.
struct RenderObject {
    VertexObject& mesh;
    ShaderProgram& material;

    RenderObject(VertexObject& mesh, ShaderProgram& material)
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
