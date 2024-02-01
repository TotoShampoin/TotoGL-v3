#pragma once

#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace TotoGL {

class Transformation {
public:
    void translate(const glm::vec3& translation) { _transform_matrix = glm::translate(_transform_matrix, translation); }
    void rotate(const float& angle, const glm::vec3& axis) { _transform_matrix = glm::rotate(_transform_matrix, angle, axis); }
    void scale(const glm::vec3& factor) { _transform_matrix = glm::scale(_transform_matrix, factor); }

    glm::mat4& matrix() { return _transform_matrix; }
    const glm::mat4& matrix() const { return _transform_matrix; }

private:
    glm::mat4 _transform_matrix { 1.f };
};

}; // namespace TotoGL
