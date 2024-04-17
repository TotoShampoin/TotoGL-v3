#include "TotoGL/Loaders/WavefrontLoader.hpp"
#include "TotoGL/Primitives/Color.hpp"
#include "TotoGL/Primitives/MaterialData.hpp"
#include "TotoGL/Primitives/Texture.hpp"
#include "TotoGL/Primitives/Vertex.hpp"
#include "TotoGL/RenderObject/MaterialObject.hpp"
#include "TotoGL/RenderObject/Mesh.hpp"
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <vector>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

namespace TotoGL {

MaterialObject loadWavefront(const std::filesystem::path& path) {
    auto totogl_meshes = std::vector<MeshInstanceId>();
    auto totogl_materials = std::vector<MaterialData>();
    auto totogl_material_indices = std::vector<uint>();

    auto parent = path.parent_path();

    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = parent.string();
    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(path.string(), reader_config)) {
        if (!reader.Error().empty()) {
            throw std::runtime_error(reader.Error());
        }
        throw std::runtime_error("Failed to parse the file");
    }

    if (!reader.Warning().empty()) {
        throw std::runtime_error(reader.Warning());
    }

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();
    auto& materials = reader.GetMaterials();

    for (const auto& shape : shapes) {
        std::vector<VertexType> vertices;
        std::vector<uint> indices;

        size_t index_offset = 0;
        size_t submesh_index = 0;
        // for (const auto& face_nb_verts : shape.mesh.num_face_vertices) {
        size_t last_material_index;
        if (shape.mesh.num_face_vertices.empty()) {
            continue;
        }
        for (size_t i = 0; i < shape.mesh.num_face_vertices.size(); i++) {
            const auto face_nb_verts = shape.mesh.num_face_vertices[i];
            if (face_nb_verts != 3) {
                throw std::runtime_error("Only triangles are supported, please triangulate your mesh");
            }
            if (i > 0) {
                if (shape.mesh.material_ids[i] != last_material_index) {
                    auto mesh = MeshFactory::create(Mesh(vertices, indices));
                    mesh->cull_face() = Mesh::CullFace::NONE;
                    auto material_index = shape.mesh.material_ids[i - 1];
                    totogl_meshes.push_back(mesh);
                    totogl_material_indices.push_back(material_index);
                    vertices.clear();
                    indices.clear();
                    submesh_index = 0;
                }
            }
            for (int v = 0; v < face_nb_verts; v++) {
                const auto& tiny_index = shape.mesh.indices[index_offset + v];
                glm::vec3 vertex;
                glm::vec3 normal;
                glm::vec2 uv;
                vertex = {
                    attrib.vertices[3 * tiny_index.vertex_index + 0],
                    attrib.vertices[3 * tiny_index.vertex_index + 1],
                    attrib.vertices[3 * tiny_index.vertex_index + 2]
                };
                if (tiny_index.normal_index >= 0) {
                    normal = {
                        attrib.normals[3 * tiny_index.normal_index + 0],
                        attrib.normals[3 * tiny_index.normal_index + 1],
                        attrib.normals[3 * tiny_index.normal_index + 2]
                    };
                }
                if (tiny_index.texcoord_index >= 0) {
                    uv = {
                        attrib.texcoords[2 * tiny_index.texcoord_index + 0],
                        attrib.texcoords[2 * tiny_index.texcoord_index + 1]
                    };
                }
                vertices.push_back({ vertex, normal, uv });
            }
            indices.push_back(submesh_index + 0);
            indices.push_back(submesh_index + 1);
            indices.push_back(submesh_index + 2);
            index_offset += face_nb_verts;
            submesh_index += face_nb_verts;
            last_material_index = shape.mesh.material_ids[i];
        }
        auto mesh = MeshFactory::create(Mesh(vertices, indices));
        mesh->cull_face() = Mesh::CullFace::NONE;
        auto material_index = shape.mesh.material_ids.back();
        totogl_meshes.push_back(mesh);
        totogl_material_indices.push_back(material_index);
    }

    for (const auto& t_material : materials) {
        MaterialData material;
        material.ambient = ColorRGB(t_material.ambient[0], t_material.ambient[1], t_material.ambient[2]);
        material.diffuse = ColorRGB(t_material.diffuse[0], t_material.diffuse[1], t_material.diffuse[2]);
        material.specular = ColorRGB(t_material.specular[0], t_material.specular[1], t_material.specular[2]);
        material.emissive = ColorRGB(t_material.emission[0], t_material.emission[1], t_material.emission[2]);
        material.shininess = (t_material.shininess > 1.f) ? t_material.shininess : 1.f;
        if (!t_material.ambient_texname.empty()) {
            auto texture = TextureFactory::create(Texture(parent / t_material.ambient_texname));
            material.ambient_texture = texture;
            if (material.ambient == ColorRGB(0, 0, 0)) {
                material.ambient = ColorRGB(1);
            }
        }
        if (!t_material.diffuse_texname.empty()) {
            auto texture = TextureFactory::create(Texture(parent / t_material.diffuse_texname));
            material.diffuse_texture = texture;
            if (material.diffuse == ColorRGB(0, 0, 0)) {
                material.diffuse = ColorRGB(1);
            }
        }
        if (!t_material.specular_texname.empty()) {
            auto texture = TextureFactory::create(Texture(parent / t_material.specular_texname));
            material.specular_texture = texture;
            if (material.specular == ColorRGB(0, 0, 0)) {
                material.specular = ColorRGB(1);
            }
        }
        if (!t_material.emissive_texname.empty()) {
            auto texture = TextureFactory::create(Texture(parent / t_material.emissive_texname));
            material.emissive_texture = texture;
            if (material.emissive == ColorRGB(0, 0, 0)) {
                material.emissive = ColorRGB(1);
            }
        }
        if (!t_material.specular_highlight_texname.empty()) {
            auto texture = TextureFactory::create(Texture(parent / t_material.specular_highlight_texname));
            material.shininess_texture = texture;
            if (material.shininess == 1.f) {
                material.shininess = 1000.f;
            }
        }
        if (!t_material.alpha_texname.empty()) {
            auto texture = TextureFactory::create(Texture(parent / t_material.alpha_texname));
            material.alpha_texture = texture;
        }
        totogl_materials.push_back(material);
    }

    auto loaded_object = MaterialObject(
        std::move(totogl_meshes),
        std::move(totogl_materials),
        std::move(totogl_material_indices));

    return loaded_object;
}

//

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
