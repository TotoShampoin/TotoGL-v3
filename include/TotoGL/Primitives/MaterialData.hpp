#pragma once

#include "TotoGL/Primitives/Color.hpp"
#include "TotoGL/Primitives/Texture.hpp"
#include <glm/glm.hpp>
#include <optional>

namespace TotoGL {

struct MaterialData {
    ColorRGBA ambient;
    ColorRGBA diffuse;
    ColorRGBA specular;
    ColorRGBA emissive;
    float shininess;
    std::optional<TextureInstanceId> ambient_texture;
    std::optional<TextureInstanceId> diffuse_texture;
    std::optional<TextureInstanceId> specular_texture;
    std::optional<TextureInstanceId> emissive_texture;
};

} // namespace TotoGL
