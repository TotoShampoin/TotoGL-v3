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

    void clearColor(const glm::vec4& color) {
        glClearColor(color.r, color.g, color.b, color.a);
    }
    void clear(bool color = true, bool depth = true, bool stencil = true) {
        glClear( //
            (color ? GL_COLOR_BUFFER_BIT : 0) | //
            (depth ? GL_DEPTH_BUFFER_BIT : 0) | //
            (stencil ? GL_STENCIL_BUFFER_BIT : 0));
    }
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
