#include "TotoGL/RenderObject/Mesh.hpp"

#include "TotoGL/GPUPointer/Buffers.hpp"
#include "TotoGL/Primitives/Vertex.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <vector>

namespace TotoGL {

Mesh::Mesh() {
    glBindVertexArray(_vao.id());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo.id());
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    glEnableVertexAttribArray(VERTEX_ATTR_UV);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo.id());
    glVertexAttribPointer(VERTEX_ATTR_POSITION, sizeof(TotoGL::VertexType::position) / sizeof(float), GL_FLOAT, GL_FALSE, sizeof(TotoGL::VertexType), (const void*)offsetof(TotoGL::VertexType, position));
    glVertexAttribPointer(VERTEX_ATTR_NORMAL, sizeof(TotoGL::VertexType::normal) / sizeof(float), GL_FLOAT, GL_FALSE, sizeof(TotoGL::VertexType), (const void*)offsetof(TotoGL::VertexType, normal));
    glVertexAttribPointer(VERTEX_ATTR_UV, sizeof(TotoGL::VertexType::uv) / sizeof(float), GL_FLOAT, GL_FALSE, sizeof(TotoGL::VertexType), (const void*)offsetof(TotoGL::VertexType, uv));
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
    glBindVertexArray(GL_NONE);
}

Mesh::Mesh(const std::vector<VertexType>& vertices, const std::vector<uint>& indices)
    : Mesh() {
    load(vertices, indices);
}

void Mesh::load(const std::vector<VertexType>& vertices, const std::vector<uint>& indices) {
    glBindBuffer(GL_ARRAY_BUFFER, _vbo.id());
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(TotoGL::VertexType), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo.id());
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint), indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
    _vertex_count = vertices.size();
    _index_count = indices.size();
}

void Mesh::draw() {
    bind();
    glEnable(GL_CULL_FACE);
    glCullFace(static_cast<GLenum>(_cull_face));
    glDrawElements(static_cast<GLenum>(_draw_method), _index_count, GL_UNSIGNED_INT, 0);
    unbind();
}

void Mesh::bind() {
    glBindVertexArray(_vao.id());
}

void Mesh::unbind() {
    glBindVertexArray(GL_NONE);
}

} // namespace TotoGL
