#pragma once

#include "TotoGL/RenderObject/Camera.hpp"
#include <glm/ext/matrix_transform.hpp>

namespace TotoGL {

class OrbitControl {
public:
    OrbitControl(float angle_x, float angle_y, float distance)
        : _angle_x(angle_x)
        , _angle_y(angle_y)
        , _distance(distance) { }

    void apply(Camera& camera) {
        camera.transformation().matrix() = //
            glm::translate(glm::mat4(1), { 0, 0, -_distance }) * //
            glm::rotate(glm::mat4(1), _angle_x, { 1, 0, 0 }) * //
            glm::rotate(glm::mat4(1), _angle_y, { 0, 1, 0 }) * glm::translate(glm::mat4(1), _position);
    }
    void move(float x, float y) {
        _angle_x += x;
        _angle_y += y;
    }

    glm::vec3& position() { return _position; }
    glm::vec3 position() const { return _position; }

    float& angle_x() { return _angle_x; }
    float angle_x() const { return _angle_x; }
    float& angle_y() { return _angle_y; }
    float angle_y() const { return _angle_y; }
    float& distance() { return _distance; }
    float distance() const { return _distance; }

private:
    glm::vec3 _position { 0, 0, 0 };
    float _angle_x { 0 };
    float _angle_y { 0 };
    float _distance { 0 };
};

} // namespace TotoGL
