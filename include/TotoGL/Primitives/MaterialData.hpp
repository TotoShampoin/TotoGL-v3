#pragma once

#include "TotoGL/Primitives/Texture.hpp"
#include <glm/glm.hpp>
#include <optional>

namespace TotoGL {

struct MaterialData {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 emissive;
    float shininess;
    std::optional<TextureInstanceId> ambient_texture;
    std::optional<TextureInstanceId> diffuse_texture;
    std::optional<TextureInstanceId> specular_texture;
    std::optional<TextureInstanceId> emissive_texture;
};

} // namespace TotoGL
