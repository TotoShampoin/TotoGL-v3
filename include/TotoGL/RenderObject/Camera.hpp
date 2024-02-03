#pragma once

#include "TotoGL/Primitives/Transformation.hpp"
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

    const glm::mat4& view() const { return _view.matrix(); }
    const glm::mat4& projection() const { return _projection; }

    glm::mat4 viewProjection() const { return _projection * _view.matrix(); }

private:
    Transformation _view;
    glm::mat4 _projection { 1.f };
};

} // namespace TotoGL
