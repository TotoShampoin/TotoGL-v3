#pragma once

#include "TotoGL/RenderObject/Camera.hpp"
#include <glm/ext/matrix_transform.hpp>

namespace TotoGL {

class TrackballControl {
public:
    TrackballControl(float angle_x, float angle_y, float distance)
        : _angle_x(angle_x)
        , _angle_y(angle_y)
        , _distance(distance) { }

    void apply(Camera& camera) {
        camera.transformation().matrix() = //
            glm::translate(glm::mat4(1), { 0, 0, -_distance }) * //
            glm::rotate(glm::mat4(1), _angle_x, { 1, 0, 0 }) * //
            glm::rotate(glm::mat4(1), _angle_y, { 0, 1, 0 });
    }
    void move(float x, float y) {
        _angle_x += x;
        _angle_y += y;
    }

    float& angle_x() { return _angle_x; }
    float angle_x() const { return _angle_x; }
    float& angle_y() { return _angle_y; }
    float angle_y() const { return _angle_y; }
    float& distance() { return _distance; }
    float distance() const { return _distance; }

private:
    float _angle_x;
    float _angle_y;
    float _distance;
};

} // namespace TotoGL
