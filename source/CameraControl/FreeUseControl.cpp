#include "TotoGL/CameraControl/FreeUseControl.hpp"

#include <TotoGL/GL.hpp>
#include <glm/geometric.hpp>

namespace TotoGL {

FreeUseControl::FreeUseControl(glm::vec2 angles, glm::vec3 position, float speed, glm::vec3 up)
    : _position(position)
    , _angles(angles)
    , _speed(speed)
    , _up(up) { }

void FreeUseControl::apply(Camera& camera) {
    camera.position() = _position;
    // camera.rotation() = { _angles.y, _angles.x, 0 };
    camera.rotation() = { 0, 0, 0 };
    glm::vec3 relative_target = { 0, 0, -1 };
    glm::mat4 camera_rotation = glm::mat4(1);
    camera_rotation = camera_rotation * glm::rotate(glm::mat4(1), _angles.x, _up);
    camera_rotation = camera_rotation * glm::rotate(glm::mat4(1), _angles.y, glm::cross(_up, relative_target));
    relative_target = camera_rotation * glm::vec4(relative_target, 1);
    glm::vec3 target = _position + relative_target;
    camera.lookAt(target, _up);
}

void FreeUseControl::update(const Seconds& delta) {
    glm::mat4 ground_rotation = glm::rotate(glm::mat4(1), _angles.x, _up);
    glm::vec3 absolute_movement = ground_rotation * glm::vec4(_action, 1);
    _position_velocity = absolute_movement * _speed;

    _position += _position_velocity * delta;
    _angles += _angles_velocity * _sensitivity;
    if (_angles.y > glm::radians(89.9f))
        _angles.y = glm::radians(89.9f);
    if (_angles.y < glm::radians(-89.9f))
        _angles.y = glm::radians(-89.9f);
    _angles_velocity = { 0, 0 };
}

void FreeUseControl::bindEvents(Window& window, std::function<bool()> focus_stolen) {
    using InputEventName::KEY;
    using InputEventName::MOUSE_BUTTON;
    using VectorEventName::CURSOR_POSITION;

    window.on(MOUSE_BUTTON, [&, focus_stolen](const InputEvent& event) {
        if (event.action == GLFW_PRESS && !focus_stolen()) {
            glfwSetInputMode(window.glfwWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    });
    window.on(CURSOR_POSITION, [&](const VectorEvent& event) {
        bool is_cursor_locked = glfwGetInputMode(window.glfwWindow(), GLFW_CURSOR) == GLFW_CURSOR_DISABLED;
        if (!is_cursor_locked) {
            return;
        }
        auto [width, height] = window.size();
        _angles_velocity.x = -event.dx / height * glm::radians(90.f);
        _angles_velocity.y = event.dy / height * glm::radians(90.f);
    });
    window.on(KEY, [&](const InputEvent& event) {
        bool is_cursor_locked = glfwGetInputMode(window.glfwWindow(), GLFW_CURSOR) == GLFW_CURSOR_DISABLED;
        if (!is_cursor_locked) {
            return;
        }
        if (event.action == GLFW_PRESS && event.button == GLFW_KEY_ESCAPE) {
            glfwSetInputMode(window.glfwWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        bool is_pressing = event.action != GLFW_RELEASE;
        if (event.button == GLFW_KEY_D)
            _action.x = is_pressing;
        if (event.button == GLFW_KEY_A)
            _action.x = -is_pressing;
        if (event.button == GLFW_KEY_SPACE)
            _action.y = is_pressing;
        if (event.button == GLFW_KEY_LEFT_SHIFT)
            _action.y = -is_pressing;
        if (event.button == GLFW_KEY_S)
            _action.z = is_pressing;
        if (event.button == GLFW_KEY_W)
            _action.z = -is_pressing;
    });
}
} // namespace TotoGL
