#include "TotoGL/RenderObject/Camera.hpp"

#include "TotoGL/Primitives/Transform.hpp"
#include <glm/glm.hpp>

namespace TotoGL {

Camera& Camera::setPersective(float fovy, float aspect, float near, float far) {
    _projection = glm::perspective(fovy, aspect, near, far);
    return *this;
}

Camera& Camera::setOrthographic(float left, float right, float bottom, float top, float near, float far) {
    _projection = glm::ortho(left, right, bottom, top, near, far);
    return *this;
}

Camera& Camera::translate(const glm::vec3& translation) {
    _transform.translate(translation);
    return *this;
}

Camera& Camera::rotate(const float& angle, const glm::vec3& axis) {
    _transform.rotate(angle, axis);
    return *this;
}

} // namespace TotoGL
