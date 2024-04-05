#include "TotoGL/Primitives/Transform.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace TotoGL {

glm::mat4 Transform::matrix() const {
    return translationMatrix() * rotationMatrix() * scalingMatrix();
}

glm::mat4 Transform::translationMatrix() const {
    return glm::translate(glm::mat4(1), _translation);
}

glm::mat4 Transform::rotationMatrix() const {
    return glm::eulerAngleXYZ(_rotation.x, _rotation.y, _rotation.z);
}

glm::mat4 Transform::scalingMatrix() const {
    return glm::scale(glm::mat4(1), _scaling);
}

Transform& Transform::translate(const glm::vec3& translation) {
    _translation += translation;
    return *this;
}

Transform& Transform::rotate(const float& angle, const glm::vec3& axis) {
    glm::mat4 rotated = glm::rotate(glm::mat4(1), angle, axis) * rotationMatrix();
    glm::extractEulerAngleXYZ(rotated, _rotation.x, _rotation.y, _rotation.z);
    return *this;
}

Transform& Transform::rotate(const glm::vec3& angles) {
    static const auto PI = glm::pi<float>();
    static const auto TAU = glm::tau<float>();
    _rotation.x = glm::mod(_rotation.x + angles.x + PI, TAU) - PI;
    _rotation.y = glm::mod(_rotation.y + angles.y + PI, TAU) - PI;
    _rotation.z = glm::mod(_rotation.z + angles.z + PI, TAU) - PI;
    return *this;
}

Transform& Transform::scale(const glm::vec3& factor) {
    _scaling *= factor;
    return *this;
}

Transform& Transform::lookAt(const glm::vec3& target, const glm::vec3& up) {
    auto normalized_target = glm::normalize(target);
    auto normalized_up = glm::normalize(up);
    if (glm::dot(normalized_target, normalized_up) == 1 || glm::dot(normalized_target, normalized_up) == -1) {
        if (glm::dot(normalized_up, { 1, 0, 0 }) != 1)
            return lookAt(target, { up.x + 1, up.y, up.z });
        else
            return lookAt(target, { up.x, up.y + 1, up.z });
    }
    auto direction = glm::inverse(glm::lookAt(_translation, target, up));
    glm::extractEulerAngleXYZ(direction, _rotation.x, _rotation.y, _rotation.z);
    return *this;
}

} // namespace TotoGL
