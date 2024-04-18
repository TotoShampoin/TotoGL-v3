#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace TotoGL {

class Transform {
public:
    glm::mat4 matrix() const;

    Transform& translate(const glm::vec3& translation);
    Transform& rotate(const float& angle, const glm::vec3& axis);
    Transform& rotate(const glm::vec3& angles);
    Transform& scale(const glm::vec3& factor);

    Transform& lookAt(const glm::vec3& target, const glm::vec3& up = { 0, 1, 0 });

    glm::vec3 front() const;
    glm::vec3 right() const;
    glm::vec3 up() const;

    glm::vec3& translation() { return _translation; }
    const glm::vec3& translation() const { return _translation; }
    glm::mat4 translationMatrix() const;

    glm::vec3& rotation() { return _rotation; }
    const glm::vec3& rotation() const { return _rotation; }
    glm::mat4 rotationMatrix() const;

    glm::vec3& scaling() { return _scaling; }
    const glm::vec3& scaling() const { return _scaling; }
    glm::mat4 scalingMatrix() const;

private:
    glm::vec3 _translation { 0. };
    glm::vec3 _rotation { 0. };
    glm::vec3 _scaling { 1. };
};

} // namespace TotoGL
