#pragma once

#include "TotoGL/Primitives/Texture.hpp"
#include <glm/glm.hpp>

namespace TotoGL {

struct MaterialData {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 emissive;
    float shininess;
    TextureInstanceId ambient_texture = TextureFactory::NULL_INSTANCE;
    TextureInstanceId diffuse_texture = TextureFactory::NULL_INSTANCE;
    TextureInstanceId specular_texture = TextureFactory::NULL_INSTANCE;
    TextureInstanceId emissive_texture = TextureFactory::NULL_INSTANCE;
};

} // namespace TotoGL
