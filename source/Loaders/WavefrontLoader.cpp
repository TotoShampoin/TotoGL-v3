#include "TotoGL/Primitives/Vertex.hpp"
#include "TotoGL/RenderObject/Mesh.hpp"
#include <vector>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

namespace TotoGL {

Mesh loadWavefrontObj(std::ifstream&& file) {
    struct VertexData {
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> uvs;
        std::vector<uint> vertices_indices;
        std::vector<uint> normals_indices;
        std::vector<uint> uvs_indices;
    } loaded_data;

    tinyobj::callback_t callback;
    callback.vertex_cb = [](void* user_data, float x, float y, float z, float w) {
        auto& vertices = static_cast<VertexData*>(user_data)->vertices;
        vertices.push_back({ x, y, z });
    };
    callback.normal_cb = [](void* user_data, float x, float y, float z) {
        auto& normals = static_cast<VertexData*>(user_data)->normals;
        normals.push_back({ x, y, z });
    };
    callback.texcoord_cb = [](void* user_data, float x, float y, float z) {
        auto& uvs = static_cast<VertexData*>(user_data)->uvs;
        uvs.push_back({ x, y });
    };
    callback.index_cb = [](void* user_data, tinyobj::index_t* indices, int num_indices) {
        auto& vertices = static_cast<VertexData*>(user_data)->vertices_indices;
        auto& normals = static_cast<VertexData*>(user_data)->normals_indices;
        auto& uvs = static_cast<VertexData*>(user_data)->uvs_indices;
        for (int i = 0; i < num_indices; i++) {
            vertices.push_back(indices[i].vertex_index - 1);
            normals.push_back(indices[i].normal_index - 1);
            uvs.push_back(indices[i].texcoord_index - 1);
        }
    };

    std::string err;
    if (!tinyobj::LoadObjWithCallback(file, callback, &loaded_data, nullptr, &err)) {
        throw std::runtime_error(err);
    }

    std::vector<VertexType> vertices;
    std::vector<uint> indices;

    for (size_t i = 0; i < loaded_data.vertices_indices.size(); i++) {
        const auto& vertex = loaded_data.vertices[loaded_data.vertices_indices[i]];
        const auto& normal = loaded_data.normals[loaded_data.normals_indices[i]];
        const auto& uv = loaded_data.uvs[loaded_data.uvs_indices[i]];
        vertices.push_back({ vertex, normal, uv });
        indices.push_back(i);
    }
    for (size_t i = 0; i < indices.size(); i += 3) {
        std::swap(indices[i], indices[i + 2]);
    }

    return Mesh { vertices, indices };
}

} // namespace TotoGL
