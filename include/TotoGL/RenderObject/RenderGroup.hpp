#pragma once

#include "TotoGL/RenderObject/RenderObject.hpp"
#include <vector>

namespace TotoGL {

class RenderGroup {
public:
    RenderGroup() = default;
    RenderGroup(std::vector<RenderObjectInstanceId>&& objects);
    ~RenderGroup();

    RenderGroup& translate(const glm::vec3& translation);
    RenderGroup& scale(const glm::vec3& factor);
    RenderGroup& rotate(const float& angle, const glm::vec3& axis);
    void lookAt(const glm::vec3& target, const glm::vec3& up = { 0, 1, 0 });

    Transform& transformation() { return _transform; }
    glm::mat4 transformMatrix() const { return _transform.matrix(); }
    glm::vec3& position() { return _transform.translation(); }
    glm::vec3& rotation() { return _transform.rotation(); }
    glm::vec3& scaling() { return _transform.scaling(); }

    const std::vector<RenderObjectInstanceId>& objects() const { return _objects; }

private:
    std::vector<RenderObjectInstanceId> _objects;
    Transform _transform;
};

using RenderGroupFactory = Factory<RenderGroup>;
using RenderGroupInstanceId = ObjectInstanceId<RenderGroup>;

} // namespace TotoGL
