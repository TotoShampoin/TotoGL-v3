#pragma once

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
    Light(glm::vec3 color, float strength, LightType type)
        : _type(type)
        , _color(color)
        , _strength(strength) { }
    ~Light() = default;

    Light& translate(const glm::vec3& translation) {
        _transform.translate(translation);
        return *this;
    }
    Light& rotate(const float& angle, const glm::vec3& axis) {
        _transform.rotate(angle, axis);
        return *this;
    }

    LightType& type() { return _type; }
    glm::vec3& color() { return _color; }
    float& strength() { return _strength; }

    Transform& transformation() { return _transform; }
    glm::vec3& position() { return _transform.translation(); }
    glm::vec3 direction() {
        glm::vec3 direction;
        direction = glm::vec3(_transform.rotationMatrix() * glm::vec4(0, 0, -1, 1));
        return direction;
    }

private:
    Transform _transform;
    LightType _type;
    glm::vec3 _color;
    float _strength;
};

} // namespace TotoGL
