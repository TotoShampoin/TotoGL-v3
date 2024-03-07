#pragma once

#include <GL/glew.h>
#include <format>
#include <stdexcept>
#include <vector>

#include "RenderObject/Camera.hpp"
#include "RenderObject/RenderObject.hpp"
#include "TotoGL/RenderObject/Light.hpp"
#include "TotoGL/RenderObject/Scene.hpp"

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
        glEnable(GL_DEPTH_TEST);
    }

    void render(TotoGL::Scene& scene, TotoGL::Camera& camera) {
        std::vector<std::reference_wrapper<TotoGL::RenderObject>> objects;
        std::vector<std::reference_wrapper<TotoGL::Light>> lights;
        for (auto& component : scene.sceneComponents()) {
            if (std::holds_alternative<std::reference_wrapper<TotoGL::RenderObject>>(component)) {
                objects.push_back(std::get<std::reference_wrapper<TotoGL::RenderObject>>(component));
            } else if (std::holds_alternative<std::reference_wrapper<TotoGL::Light>>(component)) {
                lights.push_back(std::get<std::reference_wrapper<TotoGL::Light>>(component));
            }
        }
        for (auto& object : objects) {
            render(object, camera, lights);
        }
    }

    void render(TotoGL::RenderObject& object, TotoGL::Camera& camera) {
        auto& material = object.material();
        auto modelview = camera.view() * object.transformMatrix();
        auto normal = glm::mat3(glm::transpose(glm::inverse(modelview)));
        material.uniform("u_projection", camera.projection());
        material.uniform("u_modelview", modelview);
        material.uniform("u_normal", normal);
        object.draw();
    }
    void render(TotoGL::RenderObject& object, TotoGL::Camera& camera, TotoGL::Light& light, size_t index = 0) {
        auto& material = object.material();
        const auto light_name = std::format("u_lights[{}]", index);
        material.uniform(light_name + ".color", light.color());
        material.uniform(light_name + ".strength", light.strength());
        material.uniform(light_name + ".type", static_cast<int>(light.type()));
        if (light.type() == TotoGL::LightType::POINT) {
            material.uniform(light_name + ".pos_or_dir",
                glm::vec3(camera.view() * glm::vec4(light.position(), 1)));
        } else if (light.type() == TotoGL::LightType::DIRECTIONAL) {
            material.uniform(light_name + ".pos_or_dir",
                glm::mat3(glm::transpose(glm::inverse(camera.view()))) * light.direction());
        }
        render(object, camera);
    }
    void render(TotoGL::RenderObject& object, TotoGL::Camera& camera, std::vector<std::reference_wrapper<TotoGL::Light>>& lights) {
        auto& material = object.material();
        material.uniform("u_lights_count", static_cast<int>(lights.size()));
        for (size_t i = 0; i < lights.size(); i++) {
            render(object, camera, lights[i], i);
        }
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
