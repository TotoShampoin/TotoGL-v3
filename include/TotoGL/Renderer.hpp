#pragma once

#include <vector>

#include "RenderObject/Camera.hpp"
#include "RenderObject/Light.hpp"
#include "RenderObject/RenderObject.hpp"
#include "RenderObject/Scene.hpp"

namespace TotoGL {

class Renderer {
public:
    Renderer();
    Renderer(const Renderer&) = delete;
    ~Renderer() = default;

    void clearColor(const glm::vec4& color);
    void clear(bool color = true, bool depth = true, bool stencil = true);

    void render(TotoGL::Scene& scene, TotoGL::Camera& camera);
    void renderOverrideMaterial(TotoGL::Scene& scene, TotoGL::Camera& camera, TotoGL::ShaderMaterial& material);

private:
    void init();

    void render(TotoGL::RenderObject& object, TotoGL::Camera& camera);
    void render(TotoGL::RenderObject& object, TotoGL::Camera& camera, TotoGL::Light& light, size_t index = 0);
    void render(TotoGL::RenderObject& object, TotoGL::Camera& camera, std::vector<std::reference_wrapper<TotoGL::Light>>& lights);

    void renderOverrideMaterial(TotoGL::RenderObject& object, TotoGL::Camera& camera, TotoGL::ShaderMaterial& material);
    void renderOverrideMaterial(TotoGL::RenderObject& object, TotoGL::Camera& camera, TotoGL::Light& light, TotoGL::ShaderMaterial& material, size_t index = 0);
    void renderOverrideMaterial(TotoGL::RenderObject& object, TotoGL::Camera& camera, std::vector<std::reference_wrapper<TotoGL::Light>>& lights, TotoGL::ShaderMaterial& material);
};

} // namespace TotoGL
