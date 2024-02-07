#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace TotoGL {

// TODO: This don't work, find out why
class Transform {
public:
    glm::mat4 matrix() const {
        return translationMatrix() * rotationMatrix() * scalingMatrix();
    }

    Transform& translate(const glm::vec3& translation) {
        _translation += translation;
        return *this;
    }
    Transform& rotate(const float& angle, const glm::vec3& axis) {
        glm::mat4 rotated = glm::rotate(rotationMatrix(), angle, axis);
        glm::extractEulerAngleXYZ(rotated, _rotation.x, _rotation.y, _rotation.z);
        return *this;
    }
    Transform& rotate(const glm::vec3& angles) {
        static const auto PI = glm::pi<float>();
        static const auto TAU = glm::tau<float>();
        _rotation.x = glm::mod(_rotation.x + angles.x + PI, TAU) - PI;
        _rotation.y = glm::mod(_rotation.y + angles.y + PI, TAU) - PI;
        _rotation.z = glm::mod(_rotation.z + angles.z + PI, TAU) - PI;
        return *this;
    }
    Transform& scale(const glm::vec3& factor) {
        _scaling *= factor;
        return *this;
    }

    Transform& lookAt(const glm::vec3& target, const glm::vec3& up) {
        auto direction = glm::inverse(glm::lookAt(_translation, target, up));
        glm::extractEulerAngleXYZ(direction, _rotation.x, _rotation.y, _rotation.z);
        return *this;
    }

    glm::vec3& translation() { return _translation; }
    const glm::vec3& translation() const { return _translation; }
    glm::mat4 translationMatrix() const { return glm::translate(glm::mat4(1), _translation); }

    glm::vec3& rotation() { return _rotation; }
    const glm::vec3& rotation() const { return _rotation; }
    glm::mat4 rotationMatrix() const { return glm::eulerAngleXYZ(_rotation.x, _rotation.y, _rotation.z); }

    glm::vec3& scaling() { return _scaling; }
    const glm::vec3& scaling() const { return _scaling; }
    glm::mat4 scalingMatrix() const { return glm::scale(glm::mat4(1), _scaling); }

private:
    glm::vec3 _translation { 0. };
    glm::vec3 _rotation { 0. };
    glm::vec3 _scaling { 1. };
};

} // namespace TotoGL
