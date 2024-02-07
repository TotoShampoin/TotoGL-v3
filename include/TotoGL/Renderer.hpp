#pragma once

#include <GL/glew.h>
#include <stdexcept>

#include "TotoGL/RenderObject/Camera.hpp"
#include "TotoGL/RenderObject/RenderObject.hpp"

namespace TotoGL {

class Renderer {
public:
    Renderer() {
        init();
    }
    Renderer(const Renderer&) = delete;
    ~Renderer() { }

    void render(TotoGL::RenderObject& object, TotoGL::Camera& camera) {
        auto modelview = camera.view() * object.transformMatrix();
        auto normal = glm::mat3(glm::transpose(glm::inverse(modelview)));
        auto& material = object.material();
        material.uniform("u_projection", camera.projection());
        material.uniform("u_modelview", modelview);
        material.uniform("u_normal", normal);
        object.draw();
    }

private:
    void init() {
        static bool is_glew_init = false;
        if (is_glew_init)
            return;
        if (auto error = glewInit()) {
            throw std::runtime_error(reinterpret_cast<const char*>(glewGetErrorString(error)));
        }
    }
};

} // namespace TotoGL
