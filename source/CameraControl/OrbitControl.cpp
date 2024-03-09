#include "TotoGL/CameraControl/OrbitControl.hpp"

#include "TotoGL/RenderObject/Camera.hpp"
#include "TotoGL/Window.hpp"

namespace TotoGL {

OrbitControl::OrbitControl(float angle_x, float angle_y, float distance)
    : _alpha(angle_x)
    , _beta(angle_y)
    , _distance(distance) { }

void OrbitControl::apply(Camera& camera) {
    camera.position() = _position + //
        glm::vec3(
            -glm::cos(_alpha) * glm::cos(_beta) * _distance,
            -glm::sin(_alpha) * _distance,
            glm::cos(_alpha) * glm::sin(_beta) * _distance);
    camera.lookAt(_position, { 0, 1, 0 });
}

void OrbitControl::rotate(float x, float y) {
    static constexpr auto HALF_PI = glm::half_pi<float>();
    static constexpr auto EPSILON = .0001f;
    _alpha += x;
    _beta += y;
    if (_alpha > HALF_PI - EPSILON)
        _alpha = HALF_PI - EPSILON;
    if (_alpha < -HALF_PI + EPSILON)
        _alpha = -HALF_PI + EPSILON;
    _beta = glm::mod(_beta + glm::pi<float>(), glm::tau<float>()) - glm::pi<float>();
}

void OrbitControl::bindEvents(Window& window, std::function<bool()> focus_stolen) {
    using InputEventName::MOUSE_BUTTON;
    using VectorEventName::CURSOR_POSITION;
    using VectorEventName::SCROLL;
    static constexpr auto PI = glm::pi<float>();

    static bool is_holding;

    window.on(MOUSE_BUTTON, [&](const InputEvent& event) {
        if (event.button == GLFW_MOUSE_BUTTON_1) {
            is_holding = bool(event.action);
        }
    });
    window.on(CURSOR_POSITION, [&](const VectorEvent& event) {
        if (focus_stolen()) {
            is_holding = false;
            return;
        }
        if (!is_holding)
            return;
        rotate(event.x * .01f, event.y * .01f);
    });
    window.on(SCROLL, [&](const VectorEvent& event) {
        _distance -= event.y * .1f;
        if (_distance < .1f)
            _distance = .1f;
    });
}

} // namespace TotoGL
