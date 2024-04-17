#include "TotoGL/RenderObject/Mesh.hpp"

namespace TotoGL {

Mesh Mesh::quad(float width, float height) {
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
Mesh Mesh::cube(float width, float height, float depth) {
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
        2, 1, 0, 3, 2, 0, // front
        5, 6, 4, 6, 7, 4, // back
        10, 9, 8, 11, 10, 8, // left
        13, 14, 12, 14, 15, 12, // right
        18, 17, 16, 19, 18, 16, // top
        21, 22, 20, 22, 23, 20, // bottom
    };
    return Mesh(vertices, triangles);
}

Mesh Mesh::sphere(float radius, int x_segments, int y_segments) {
    static const float PI = glm::pi<float>();
    static const float TAU = glm::tau<float>();

    std::vector<VertexType> vertices;
    std::vector<uint> indices;
    for (int y = 0; y <= y_segments; y++) {
        for (int x = 0; x <= x_segments; x++) {
            glm::vec2 uv = {
                -(float)x / x_segments,
                (float)y / y_segments
            };
            glm::vec3 coord = {
                glm::cos(-uv.x * TAU) * glm::sin(uv.y * PI),
                glm::cos(uv.y * PI),
                glm::sin(-uv.x * TAU) * glm::sin(uv.y * PI),
            };
            vertices.push_back({ coord * radius, coord, uv });

            if (x < x_segments && y < y_segments) {
                indices.push_back((y + 1) * (x_segments + 1) + x);
                indices.push_back(y * (x_segments + 1) + x + 1);
                indices.push_back(y * (x_segments + 1) + x);

                indices.push_back((y + 1) * (x_segments + 1) + x);
                indices.push_back((y + 1) * (x_segments + 1) + x + 1);
                indices.push_back(y * (x_segments + 1) + x + 1);
            }
        }
    }
    return Mesh(vertices, indices);
}

} // namespace TotoGL
