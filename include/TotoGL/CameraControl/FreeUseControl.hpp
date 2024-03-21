#pragma once

#include "TotoGL/Primitives/Clock.hpp"
#include "TotoGL/RenderObject/Camera.hpp"
#include "TotoGL/Window.hpp"
#include <glm/fwd.hpp>

namespace TotoGL {

class FreeUseControl {
public:
    FreeUseControl(glm::vec2 angles = { 0, 0 }, glm::vec3 position = { 0, 0, 0 }, float speed = 1, glm::vec3 up = { 0, 1, 0 });

    void apply(Camera& camera);
    void update(const Seconds&);

    void bindEvents(Window& window, std::function<bool()> focus_stolen = empty_function);

    glm::vec3& position() { return _position; }
    glm::vec2& angles() { return _angles; }
    glm::vec3& positionVelocity() { return _position_velocity; }
    glm::vec2& anglesVelocity() { return _angles_velocity; }
    float& speed() { return _speed; }
    float& sensitivity() { return _sensitivity; }

    glm::vec3 position() const { return _position; }
    glm::vec2 angles() const { return _angles; }
    glm::vec3 positionVelocity() const { return _position_velocity; }
    glm::vec2 anglesVelocity() const { return _angles_velocity; }
    float speed() const { return _speed; }
    float sensitivity() const { return _sensitivity; }

private:
    glm::vec3 _position { 0, 0, 0 };
    glm::vec2 _angles { 0, 0 };

    glm::vec3 _position_velocity { 0, 0, 0 };
    glm::vec2 _angles_velocity { 0, 0 };

    glm::vec3 _up { 0, 1, 0 };
    float _speed { 1 };
    float _sensitivity { 1 };

    glm::vec3 _action { 0, 0, 0 };

    static constexpr auto empty_function = [] { return false; };
};

} // namespace TotoGL
