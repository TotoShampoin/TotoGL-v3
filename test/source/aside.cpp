#define TOTOGL_IMPLEMENTATIONS
#include <TotoGL/TotoGL.hpp>

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
    static const auto tex_id = TotoGL::TextureFactory::create(
        // TotoGL::Texture(std::ifstream("assets/textures/kirby.png")));
        TotoGL::Texture(std::ifstream("assets/textures/earth.jpg")));
    static const auto mesh_id = TotoGL::MeshFactory::create(
        TotoGL::Mesh::sphere(1, 12, 8));
    static const auto mat_id = [&]() {
        const auto mat_id = TotoGL::ShaderMaterialFactory::create(
            TotoGL::ShaderMaterial(
                std::ifstream("assets/shader/shader.vert"),
                std::ifstream("assets/shader/phong.frag")));
        auto& material = TotoGL::ShaderMaterialFactory::get(mat_id);
        auto& texture = TotoGL::TextureFactory::get(tex_id);
        material.uniform("u_texture", texture);
        return mat_id;
    }();

    const auto obj_id = TotoGL::RenderObjectFactory::create(TotoGL::RenderObject(mesh_id, mat_id));
    return obj_id;
}

TotoGL::RenderObjectInstanceId makeHelper() {
    static const auto mesh_id = TotoGL::MeshFactory::create(
        TotoGL::Mesh::sphere());
    static const auto mat_id = TotoGL::ShaderMaterialFactory::create(
        TotoGL::ShaderMaterial(
            std::ifstream("assets/shader/shader.vert"),
            std::ifstream("assets/shader/uv.frag")));
    const auto obj_id = TotoGL::RenderObjectFactory::create(TotoGL::RenderObject(mesh_id, mat_id));
    return obj_id;
}
