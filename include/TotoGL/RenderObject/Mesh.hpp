#pragma once

#include "TotoGL/GPUPointer/Buffers.hpp"
#include "TotoGL/Misc/Factory.hpp"
#include "TotoGL/Primitives/Vertex.hpp"
#include <glm/glm.hpp>
#include <vector>

namespace TotoGL {

class Mesh {
public:
    enum class CullFace {
        FRONT = GL_FRONT,
        BACK = GL_BACK,
        DOUBLE = GL_FRONT_AND_BACK
    };
    enum class DrawMethod {
        POINTS = GL_POINTS,
        LINES = GL_LINES,
        LINE_STRIP = GL_LINE_STRIP,
        LINE_LOOP = GL_LINE_LOOP,
        TRIANGLES = GL_TRIANGLES,
        TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
        TRIANGLE_FAN = GL_TRIANGLE_FAN,
    };

    Mesh() {
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

    Mesh(const std::vector<VertexType>& vertices, const std::vector<uint>& indices)
        : Mesh() {
        load(vertices, indices);
    }

    void load(const std::vector<VertexType>& vertices, const std::vector<uint>& indices) {
        glBindBuffer(GL_ARRAY_BUFFER, _vbo.id());
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(TotoGL::VertexType), vertices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo.id());
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint), indices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
        _vertex_count = vertices.size();
        _index_count = indices.size();
    }

    void draw() {
        bind();
        glEnable(GL_CULL_FACE);
        glCullFace(static_cast<GLenum>(_cull_face));
        glDrawElements(static_cast<GLenum>(_draw_method), _index_count, GL_UNSIGNED_INT, 0);
        unbind();
    }

    void bind() {
        glBindVertexArray(_vao.id());
    }

    static void unbind() {
        glBindVertexArray(0);
    }

    // geometry digures

    static Mesh quad(float width = 1.f, float height = 1.f) {
        static std::vector<VertexType> vertices = {
            { { -width / 2.f, height / 2.f, 0 }, { 0, 0, 1 }, { 0, 0 } },
            { { width / 2.f, height / 2.f, 0 }, { 0, 0, 1 }, { 1, 0 } },
            { { width / 2.f, -height / 2.f, 0 }, { 0, 0, 1 }, { 1, 1 } },
            { { -width / 2.f, -height / 2.f, 0 }, { 0, 0, 1 }, { 0, 1 } },
        };
        static std::vector<uint> triangles = { 0, 1, 2, 0, 2, 3 };
        return Mesh(vertices, triangles);
    }

    // each face independent from the other
    static Mesh cube(float width = 1.f, float height = 1.f, float depth = 1.f) {
        static std::vector<VertexType> vertices = {
            // front
            { { -width / 2.f, height / 2.f, depth / 2.f }, { 0, 0, 1 }, { 0, 0 } },
            { { width / 2.f, height / 2.f, depth / 2.f }, { 0, 0, 1 }, { 1, 0 } },
            { { width / 2.f, -height / 2.f, depth / 2.f }, { 0, 0, 1 }, { 1, 1 } },
            { { -width / 2.f, -height / 2.f, depth / 2.f }, { 0, 0, 1 }, { 0, 1 } },
            // back
            { { -width / 2.f, height / 2.f, -depth / 2.f }, { 0, 0, -1 }, { 1, 0 } },
            { { width / 2.f, height / 2.f, -depth / 2.f }, { 0, 0, -1 }, { 0, 0 } },
            { { width / 2.f, -height / 2.f, -depth / 2.f }, { 0, 0, -1 }, { 0, 1 } },
            { { -width / 2.f, -height / 2.f, -depth / 2.f }, { 0, 0, -1 }, { 1, 1 } },
            // left
            { { -width / 2.f, height / 2.f, -depth / 2.f }, { -1, 0, 0 }, { 0, 0 } },
            { { -width / 2.f, height / 2.f, depth / 2.f }, { -1, 0, 0 }, { 1, 0 } },
            { { -width / 2.f, -height / 2.f, depth / 2.f }, { -1, 0, 0 }, { 1, 1 } },
            { { -width / 2.f, -height / 2.f, -depth / 2.f }, { -1, 0, 0 }, { 0, 1 } },
            // right
            { { width / 2.f, height / 2.f, -depth / 2.f }, { 1, 0, 0 }, { 1, 0 } },
            { { width / 2.f, height / 2.f, depth / 2.f }, { 1, 0, 0 }, { 0, 0 } },
            { { width / 2.f, -height / 2.f, depth / 2.f }, { 1, 0, 0 }, { 0, 1 } },
            { { width / 2.f, -height / 2.f, -depth / 2.f }, { 1, 0, 0 }, { 1, 1 } },
            // top
            { { -width / 2.f, height / 2.f, -depth / 2.f }, { 0, 1, 0 }, { 0, 0 } },
            { { width / 2.f, height / 2.f, -depth / 2.f }, { 0, 1, 0 }, { 1, 0 } },
            { { width / 2.f, height / 2.f, depth / 2.f }, { 0, 1, 0 }, { 1, 1 } },
            { { -width / 2.f, height / 2.f, depth / 2.f }, { 0, 1, 0 }, { 0, 1 } },
            // bottom
            { { -width / 2.f, -height / 2.f, -depth / 2.f }, { 0, -1, 0 }, { 1, 0 } },
            { { width / 2.f, -height / 2.f, -depth / 2.f }, { 0, -1, 0 }, { 0, 0 } },
            { { width / 2.f, -height / 2.f, depth / 2.f }, { 0, -1, 0 }, { 0, 1 } },
            { { -width / 2.f, -height / 2.f, depth / 2.f }, { 0, -1, 0 }, { 1, 1 } },
        };
        static std::vector<uint> triangles = {
            0, 1, 2, 0, 2, 3, // front
            4, 6, 5, 4, 7, 6, // back
            8, 9, 10, 8, 10, 11, // left
            12, 14, 13, 12, 15, 14, // right
            16, 17, 18, 16, 18, 19, // top
            20, 22, 21, 20, 23, 22, // bottom
        };
        return Mesh(vertices, triangles);
    }

    CullFace& cull_face() { return _cull_face; };
    DrawMethod& draw_method() { return _draw_method; };

private:
    VboId _vbo;
    IboId _ibo;
    VaoId _vao;
    size_t _vertex_count, _index_count;

    CullFace _cull_face = CullFace::FRONT;
    DrawMethod _draw_method = DrawMethod::TRIANGLES;

    static constexpr uint VERTEX_ATTR_POSITION = 0;
    static constexpr uint VERTEX_ATTR_NORMAL = 1;
    static constexpr uint VERTEX_ATTR_UV = 2;
};

using MeshFactory = Factory<Mesh>;

} // namespace TotoGL
