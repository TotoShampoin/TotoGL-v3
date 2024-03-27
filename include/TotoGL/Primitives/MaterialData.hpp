#pragma once

#include "TotoGL/Primitives/Texture.hpp"
#include <glm/glm.hpp>
#include <optional>

namespace TotoGL {

struct MaterialData {
    glm::vec4 ambient;
    glm::vec4 diffuse;
    glm::vec4 specular;
    glm::vec4 emissive;
    float shininess;
    std::optional<TextureInstanceId> ambient_texture;
    std::optional<TextureInstanceId> diffuse_texture;
    std::optional<TextureInstanceId> specular_texture;
    std::optional<TextureInstanceId> emissive_texture;
};

} // namespace TotoGL
