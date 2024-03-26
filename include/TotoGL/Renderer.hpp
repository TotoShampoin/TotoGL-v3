#pragma once

#include <optional>
#include <vector>

#include "RenderObject/Camera.hpp"
#include "RenderObject/Light.hpp"
#include "RenderObject/RenderObject.hpp"
#include "RenderObject/Scene.hpp"
#include "TotoGL/RenderObject/Mesh.hpp"
#include "TotoGL/RenderObject/ShaderMaterial.hpp"

namespace TotoGL {

class Renderer {
public:
    Renderer();
    Renderer(const Renderer&) = delete;
    ~Renderer() = default;

    void clearColor(const glm::vec4& color);
    void clear(bool color = true, bool depth = true, bool stencil = true);

    void render(
        TotoGL::Scene&, TotoGL::Camera&,
        std::optional<std::reference_wrapper<TotoGL::ShaderMaterial>> = std::nullopt);

    void render(
        TotoGL::RenderObject&, TotoGL::Camera&,
        std::optional<std::vector<std::reference_wrapper<TotoGL::Light>>> = std::nullopt,
        std::optional<std::reference_wrapper<TotoGL::ShaderMaterial>> = std::nullopt);

private:
    void init();

    void draw(TotoGL::Mesh& mesh, TotoGL::ShaderMaterial& material);
    void applyCamera(TotoGL::Camera& camera, TotoGL::Mesh& mesh, TotoGL::ShaderMaterial& material, TotoGL::Transform& transform);
    void applyLights(std::vector<std::reference_wrapper<TotoGL::Light>>& lights, TotoGL::ShaderMaterial& material, TotoGL::Camera& camera);
    void applyLight(TotoGL::Light& light, TotoGL::ShaderMaterial& material, TotoGL::Camera& camera, size_t index = 0);
};

} // namespace TotoGL
