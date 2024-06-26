#pragma once

#include "TotoGL/Misc/Factory.hpp"
#include "TotoGL/Primitives/Color.hpp"
#include "TotoGL/Primitives/Transform.hpp"
#include <glm/glm.hpp>

namespace TotoGL {

enum class LightType {
    AMBIENT,
    POINT,
    DIRECTIONAL,
};

class Light {
public:
    Light() = delete;
    Light(ColorRGB color, float strength, LightType type);
    ~Light() = default;

    Light& translate(const glm::vec3& translation);
    Light& rotate(const float& angle, const glm::vec3& axis);

    LightType& type() { return _type; }
    ColorRGB& color() { return _color; }
    float& strength() { return _strength; }

    Transform& transformation() { return _transform; }
    glm::vec3& position() { return _transform.translation(); }
    glm::vec3 direction();
    void setDirection(const glm::vec3& direction);

private:
    Transform _transform;
    LightType _type;
    ColorRGB _color;
    float _strength;
};

using LightFactory = Factory<Light>;
using LightInstanceId = ObjectInstanceId<Light>;

} // namespace TotoGL
