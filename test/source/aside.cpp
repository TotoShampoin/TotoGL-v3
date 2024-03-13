#define TOTOGL_IMPLEMENTATIONS
#include <TotoGL/TotoGL.hpp>

#include "TotoGL/Loaders/WavefrontLoader.hpp"

void event(
    TotoGL::Window& window,
    TotoGL::Camera& camera,
    TotoGL::FreeUseControl& control) {

    using TotoGL::VectorEventName::FRAMEBUFFER_SIZE;

    constexpr float FOV = glm::radians(70.f);

    control.bindEvents(window);
    window.on(FRAMEBUFFER_SIZE, [&](const TotoGL::VectorEvent& event) {
        glViewport(0, 0, int(event.x), int(event.y));
        camera.setPersective(FOV, (float)event.x / event.y, 0.1f, 100.f);
    });
}

TotoGL::RenderObjectInstanceId makeObject() {
    static const auto texture = TotoGL::TextureFactory::create(
        TotoGL::Texture(
            std::ifstream("assets/textures/earth.jpg")));

    static const auto mesh = TotoGL::MeshFactory::create(
        TotoGL::loadWavefrontObj(
            std::ifstream("assets/obj/cone.obj")));

    static const auto material = [&]() {
        const auto material = TotoGL::ShaderMaterialFactory::create(
            TotoGL::ShaderMaterial(
                std::ifstream("assets/shader/shader.vert"),
                std::ifstream("assets/shader/phong.frag")));
        material->uniform("u_texture", texture);
        return material;
    }();

    const auto object = TotoGL::RenderObjectFactory::create(
        TotoGL::RenderObject(mesh, material));
    return object;
}

TotoGL::RenderObjectInstanceId makeHelper() {
    static const auto mesh = TotoGL::MeshFactory::create(
        TotoGL::Mesh::sphere());
    static const auto material = TotoGL::ShaderMaterialFactory::create(
        TotoGL::ShaderMaterial(
            std::ifstream("assets/shader/shader.vert"),
            std::ifstream("assets/shader/uv.frag")));
    const auto helper = TotoGL::RenderObjectFactory::create(
        TotoGL::RenderObject(mesh, material));
    return helper;
}
