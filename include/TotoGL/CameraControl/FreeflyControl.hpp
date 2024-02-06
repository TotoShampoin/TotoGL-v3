#pragma once

#include "TotoGL/RenderObject/Camera.hpp"

namespace TotoGL {

class FreeflyControl {
public:
    FreeflyControl(float rotation_x, float rotation_y)
        : _rotation_x(rotation_x)
        , _rotation_y(rotation_y) { }

    void apply(Camera& camera) {
        auto target = glm::vec3(
            glm::cos(_rotation_y) * glm::sin(_rotation_x),
            glm::sin(_rotation_y),
            glm::cos(_rotation_y) * glm::cos(_rotation_x));
        camera.transformation().matrix() = glm::lookAt(glm::vec3(0), target, glm::vec3(0, 1, 0)) * glm::translate(glm::mat4(1), _position);
    }
    void move(float x, float y) {
        _rotation_x += x;
        _rotation_y += y;
        if (_rotation_y > glm::half_pi<float>())
            _rotation_y = glm::half_pi<float>();
        if (_rotation_y < -glm::half_pi<float>())
            _rotation_y = -glm::half_pi<float>();
        _rotation_x = glm::mod(_rotation_x + glm::pi<float>(), glm::tau<float>()) - glm::pi<float>();
    }

    glm::vec3& position() { return _position; }
    glm::vec3 position() const { return _position; }

    float& rotationX() { return _rotation_x; }
    float& rotationY() { return _rotation_y; }

    float rotationX() const { return _rotation_x; }
    float rotationY() const { return _rotation_y; }

private:
    glm::vec3 _position;
    float _rotation_x;
    float _rotation_y;
};

} // namespace TotoGL
