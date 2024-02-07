#pragma once

#include "TotoGL/Primitives/Transform.hpp"
#include <glm/glm.hpp>

namespace TotoGL {

class Camera {
public:
    Camera() = default;
    ~Camera() = default;

    static Camera Perspective(float fovy, float aspect, float near, float far) {
        return Camera().setPersective(fovy, aspect, near, far);
    }

    static Camera Orthographic(float left, float right, float bottom, float top, float near, float far) {
        return Camera().setOrthographic(left, right, bottom, top, near, far);
    }

    Camera& setPersective(float fovy, float aspect, float near, float far) {
        _projection = glm::perspective(fovy, aspect, near, far);
        return *this;
    }

    Camera& setOrthographic(float left, float right, float bottom, float top, float near, float far) {
        _projection = glm::ortho(left, right, bottom, top, near, far);
        return *this;
    }

    Camera& translate(const glm::vec3& translation) {
        _transform.translate(translation);
        return *this;
    }
    Camera& rotate(const float& angle, const glm::vec3& axis) {
        _transform.rotate(angle, axis);
        return *this;
    }

    Transform& transformation() { return _transform; }
    glm::vec3& position() { return _transform.translation(); }
    glm::vec3& rotation() { return _transform.rotation(); }
    void lookAt(const glm::vec3& target, const glm::vec3& up = { 0, 1, 0 }) {
        _transform.lookAt(target, up);
    }

    glm::mat4 view() const { return glm::inverse(_transform.matrix()); }
    glm::mat4 projection() const { return _projection; }

    glm::mat4 viewProjection() const { return projection() * view(); }

private:
    Transform _transform;
    glm::mat4 _projection { 1.f };
};

} // namespace TotoGL
