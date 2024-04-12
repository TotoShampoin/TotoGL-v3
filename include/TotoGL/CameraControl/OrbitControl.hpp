#pragma once

#include "TotoGL/RenderObject/Camera.hpp"
#include "TotoGL/Window.hpp"

namespace TotoGL {

class OrbitControl {
public:
    OrbitControl(float angle_x, float angle_y, float distance);

    void apply(Camera& camera);

    void rotate(float x, float y);

    void bindEvents(Window& window, std::function<bool()> focus_stolen = empty_function, bool steal_cursor = false);

    glm::vec3 front(const bool& grounded = false);
    glm::vec3 right(const bool& grounded = false);
    glm::vec3 up(const bool& grounded = false);

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

    static constexpr auto empty_function = [] { return false; };
};

} // namespace TotoGL
