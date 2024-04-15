#include "TotoGL/RenderObject/Light.hpp"

#include "TotoGL/Primitives/Transform.hpp"
#include <glm/glm.hpp>

namespace TotoGL {

Light::Light(ColorRGB color, float strength, LightType type)
    : _type(type)
    , _color(color)
    , _strength(strength) { }

Light& Light::translate(const glm::vec3& translation) {
    _transform.translate(translation);
    return *this;
}

Light& Light::rotate(const float& angle, const glm::vec3& axis) {
    _transform.rotate(angle, axis);
    return *this;
}

glm::vec3 Light::direction() {
    glm::vec3 direction;
    direction = glm::vec3(_transform.rotationMatrix() * glm::vec4(0, 0, -1, 1));
    return direction;
}

void Light::setDirection(const glm::vec3& direction) {
    _transform.lookAt(_transform.translation() + direction);
}

} // namespace TotoGL
