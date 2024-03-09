#pragma once

#include "TotoGL/GPUPointer/Buffers.hpp"
#include "TotoGL/Misc/Factory.hpp"
#include "TotoGL/Primitives/Vertex.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
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

    Mesh();

    Mesh(const std::vector<VertexType>& vertices, const std::vector<uint>& indices);

    void load(const std::vector<VertexType>& vertices, const std::vector<uint>& indices);

    void draw();

    void bind();

    static void unbind();

    CullFace& cull_face() { return _cull_face; };
    DrawMethod& draw_method() { return _draw_method; };

    // geometry digures
    static Mesh quad(float width = 1.f, float height = 1.f);
    static Mesh cube(float width = 1.f, float height = 1.f, float depth = 1.f);
    static Mesh sphere(float radius = 1.f, int x_segments = 64, int y_segments = 64);

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
using MeshInstanceId = ObjectInstanceId<Mesh>;

} // namespace TotoGL
