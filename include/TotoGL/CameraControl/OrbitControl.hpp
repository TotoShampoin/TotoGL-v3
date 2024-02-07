#pragma once

#include "TotoGL/RenderObject/Camera.hpp"

namespace TotoGL {

class OrbitControl {
public:
    OrbitControl(float angle_x, float angle_y, float distance)
        : _alpha(angle_x)
        , _beta(angle_y)
        , _distance(distance) { }

    void apply(Camera& camera) {
        camera.position() = _position + //
            glm::vec3(
                -glm::cos(_alpha) * glm::cos(_beta) * _distance,
                -glm::sin(_alpha) * _distance,
                glm::cos(_alpha) * glm::sin(_beta) * _distance);
        camera.lookAt(_position, { 0, 1, 0 });
    }
    void rotate(float x, float y) {
        static constexpr auto HALF_PI = glm::half_pi<float>();
        static constexpr auto EPSILON = glm::epsilon<float>();
        _alpha += x;
        _beta += y;
        if (_alpha > HALF_PI - EPSILON)
            _alpha = HALF_PI - EPSILON;
        if (_alpha < -HALF_PI + EPSILON)
            _alpha = -HALF_PI + EPSILON;
        _beta = glm::mod(_beta + glm::pi<float>(), glm::tau<float>()) - glm::pi<float>();
    }

    glm::vec3& position() { return _position; }
    glm::vec3 position() const { return _position; }

    float& angle_x() { return _alpha; }
    float angle_x() const { return _alpha; }
    float& angle_y() { return _beta; }
    float angle_y() const { return _beta; }
    float& distance() { return _distance; }
    float distance() const { return _distance; }

private:
    glm::vec3 _position { 0, 0, 0 };
    float _alpha { 0 };
    float _beta { 0 };
    float _distance { 0 };
};

} // namespace TotoGL
