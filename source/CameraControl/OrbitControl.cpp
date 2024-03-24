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

    window.on(MOUSE_BUTTON, [&, focus_stolen](const InputEvent& event) {
        if (event.button == GLFW_MOUSE_BUTTON_1) {
            is_holding = bool(event.action);
        }
    });
    window.on(CURSOR_POSITION, [&, focus_stolen](const VectorEvent& event) {
        if (focus_stolen()) {
            return;
        }
        if (!is_holding)
            return;
        auto [width, height] = window.size();
        rotate(-event.dy / height * PI, -event.dx / height * PI);
    });
    window.on(SCROLL, [&, focus_stolen](const VectorEvent& event) {
        if (focus_stolen()) {
            return;
        }
        _distance *= glm::pow(1.25, -event.dy);
        if (_distance < glm::epsilon<float>())
            _distance = glm::epsilon<float>();
    });
}

} // namespace TotoGL
