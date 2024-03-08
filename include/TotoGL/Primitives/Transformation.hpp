#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace TotoGL {

/**
 * @deprecated Use Transform.hpp instead.
 *
 */
class Transformation {
public:
    Transformation& translate(const glm::vec3& translation) {
        _transform_matrix = glm::translate(_transform_matrix, translation);
        return *this;
    }
    Transformation& rotate(const float& angle, const glm::vec3& axis) {
        _transform_matrix = glm::rotate(_transform_matrix, angle, axis);
        return *this;
    }
    Transformation& scale(const glm::vec3& factor) {
        _transform_matrix = glm::scale(_transform_matrix, factor);
        return *this;
    }

    glm::mat4& matrix() { return _transform_matrix; }
    const glm::mat4& matrix() const { return _transform_matrix; }

private:
    glm::mat4 _transform_matrix { 1.f };
};

}; // namespace TotoGL
