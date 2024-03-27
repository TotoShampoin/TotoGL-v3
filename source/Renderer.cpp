#include "TotoGL/Renderer.hpp"

#include <TotoGL/GL.hpp> // MUST STAY ON TOP

#include <format>
#include <stdexcept>
#include <vector>

#include "TotoGL/RenderObject/Camera.hpp"
#include "TotoGL/RenderObject/Light.hpp"
#include "TotoGL/RenderObject/RenderObject.hpp"
#include "TotoGL/RenderObject/Scene.hpp"
#include "TotoGL/RenderObject/Skydome.hpp"

namespace TotoGL {

Renderer::Renderer() {
    init();
}

void Renderer::init() {
    static bool is_glew_init = false;
    if (is_glew_init)
        return;
    if (auto error = glewInit()) {
        throw std::runtime_error(reinterpret_cast<const char*>(glewGetErrorString(error)));
    }
}

void Renderer::clearColor(const glm::vec4& color) {
    glClearColor(color.r, color.g, color.b, color.a);
}

void Renderer::clear(bool color, bool depth, bool stencil) {
    glClear( //
        (color ? GL_COLOR_BUFFER_BIT : 0) | //
        (depth ? GL_DEPTH_BUFFER_BIT : 0) | //
        (stencil ? GL_STENCIL_BUFFER_BIT : 0));
    glEnable(GL_DEPTH_TEST);
}

//  This will come automatically, when we'll implement object loading
void Renderer::render(
    TotoGL::Scene& scene, TotoGL::Camera& camera,
    std::optional<std::reference_wrapper<TotoGL::ShaderMaterial>> alternate_material) {
    std::vector<std::reference_wrapper<TotoGL::RenderObject>> objects;
    std::vector<std::reference_wrapper<TotoGL::Light>> lights;
    std::vector<std::reference_wrapper<TotoGL::Skydome>> skydomes;
    for (auto& component : scene.sceneComponents()) {
        if (std::holds_alternative<std::reference_wrapper<TotoGL::RenderObject>>(component)) {
            objects.push_back(std::get<std::reference_wrapper<TotoGL::RenderObject>>(component));
        } else if (std::holds_alternative<std::reference_wrapper<TotoGL::Light>>(component)) {
            lights.push_back(std::get<std::reference_wrapper<TotoGL::Light>>(component));
        } else if (std::holds_alternative<std::reference_wrapper<TotoGL::Skydome>>(component)) {
            skydomes.push_back(std::get<std::reference_wrapper<TotoGL::Skydome>>(component));
        }
    }
    clear();
    if (!alternate_material.has_value() && !skydomes.empty()) {
        render(skydomes[0].get().object(), camera);
        clear(false, true, false);
    }
    for (auto& object : objects) {
        render(object, camera, lights, alternate_material);
    }
}

void Renderer::render(
    TotoGL::RenderObject& object, TotoGL::Camera& camera,
    std::optional<std::vector<std::reference_wrapper<TotoGL::Light>>> lights,
    std::optional<std::reference_wrapper<TotoGL::ShaderMaterial>> alternate_material) {
    auto& mesh = object.mesh();
    auto& material = alternate_material.has_value() ? alternate_material.value().get() : object.material();
    auto& transform = object.transformation();
    applyCamera(camera, mesh, material, transform);
    if (lights.has_value()) {
        applyLights(lights.value(), material, camera);
    }
    draw(mesh, material);
}

void Renderer::draw(TotoGL::Mesh& mesh, TotoGL::ShaderMaterial& material) {
    material.use();
    mesh.draw();
    material.unuse();
}

void Renderer::applyCamera(TotoGL::Camera& camera, TotoGL::Mesh& mesh, TotoGL::ShaderMaterial& material, TotoGL::Transform& transform) {
    auto modelview = camera.view() * transform.matrix();
    auto normal = glm::mat3(glm::transpose(glm::inverse(modelview)));
    material.uniform("u_projection", camera.projection());
    material.uniform("u_modelview", modelview);
    material.uniform("u_normal", normal);
}

void Renderer::applyLights(std::vector<std::reference_wrapper<TotoGL::Light>>& lights, TotoGL::ShaderMaterial& material, TotoGL::Camera& camera) {
    material.uniform("u_lights_count", static_cast<int>(lights.size()));
    for (size_t i = 0; i < lights.size(); i++) {
        applyLight(lights[i], material, camera, i);
    }
}

void Renderer::applyLight(TotoGL::Light& light, TotoGL::ShaderMaterial& material, TotoGL::Camera& camera, size_t index) {
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
}

} // namespace TotoGL
