#include "TotoGL/CameraControl/OrbitControl.hpp"

#include "TotoGL/RenderObject/Camera.hpp"
#include "TotoGL/Window.hpp"
#include <glm/exponential.hpp>

namespace TotoGL {

OrbitControl::OrbitControl(float angle_x, float angle_y, float distance)
    : _alpha(angle_x)
    , _beta(angle_y)
    , _distance(distance) { }

void OrbitControl::apply(Camera& camera) {
    camera.position() = _position - front() * _distance;
    camera.lookAt(_position, { 0, 1, 0 });
}

void OrbitControl::rotate(float x, float y) {
    static constexpr auto HALF_PI = glm::half_pi<float>();
    static constexpr auto EPSILON = .0001f;
    _alpha += x;
    _beta += y;
    _alpha = glm::mod(_alpha + glm::pi<float>(), glm::tau<float>()) - glm::pi<float>();
    if (_beta > HALF_PI - EPSILON)
        _beta = HALF_PI - EPSILON;
    if (_beta < -HALF_PI + EPSILON)
        _beta = -HALF_PI + EPSILON;
}

void OrbitControl::bindEvents(Window& window, std::function<bool()> focus_stolen, bool steal_cursor) {
    using InputEventName::KEY;
    using InputEventName::MOUSE_BUTTON;
    using VectorEventName::CURSOR_POSITION;
    using VectorEventName::SCROLL;
    static constexpr auto PI = glm::pi<float>();

    static bool is_holding;

    window.on(MOUSE_BUTTON, [&, focus_stolen, steal_cursor](const InputEvent& event) {
        if (event.button == GLFW_MOUSE_BUTTON_1) {
            if (!steal_cursor && !focus_stolen()) {
                is_holding = bool(event.action);
                return;
            }
            if (event.action == GLFW_PRESS && !focus_stolen()) {
                glfwSetInputMode(window.glfwWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                is_holding = true;
            }
        }
    });
    window.on(CURSOR_POSITION, [&](const VectorEvent& event) {
        if (!is_holding)
            return;
        auto [width, height] = window.size();
        rotate(event.dx / height * PI, -event.dy / height * PI);
    });
    window.on(SCROLL, [&, focus_stolen](const VectorEvent& event) {
        if (focus_stolen()) {
            return;
        }
        _distance *= glm::pow(1.25, -event.dy);
        if (_distance < glm::epsilon<float>())
            _distance = glm::epsilon<float>();
    });
    window.on(KEY, [&, focus_stolen, steal_cursor](const InputEvent& event) {
        if (focus_stolen()) {
            return;
        }
        if (event.action == GLFW_PRESS && event.button == GLFW_KEY_ESCAPE) {
            if (steal_cursor) {
                glfwSetInputMode(window.glfwWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
            is_holding = false;
        }
    });
}

glm::vec3 OrbitControl::front(const bool& grounded) {
    if (!grounded)
        return glm::vec3(
            glm::sin(_alpha) * glm::cos(_beta),
            glm::sin(_beta),
            -glm::cos(_alpha) * glm::cos(_beta));
    return glm::vec3(
        glm::sin(_alpha),
        0,
        -glm::cos(_alpha));
}

glm::vec3 OrbitControl::right(const bool& grounded) {
    if (!grounded)
        return glm::vec3(
            glm::cos(_alpha),
            0,
            glm::sin(_alpha));
    return glm::vec3(
        glm::cos(_alpha),
        0,
        glm::sin(_alpha));
}

glm::vec3 OrbitControl::up(const bool& grounded) {
    return -glm::cross(front(grounded), right(grounded));
}

} // namespace TotoGL
