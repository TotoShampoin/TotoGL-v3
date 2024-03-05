#define TOTOGL_IMPLEMENTATIONS
#include <TotoGL/TotoGL.hpp>

void event(
    TotoGL::Window& window,
    TotoGL::Camera& camera,
    bool& holding,
    TotoGL::OrbitControl& orbit,
    glm::vec3& velocity) {

    using TotoGL::InputEventName::KEY;
    using TotoGL::InputEventName::MOUSE_BUTTON;
    using TotoGL::VectorEventName::CURSOR_POSITION;
    using TotoGL::VectorEventName::FRAMEBUFFER_SIZE;
    using TotoGL::VectorEventName::SCROLL;

    // constexpr int WIDTH = 640;
    constexpr int HEIGHT = 480;
    constexpr float FOV = glm::radians(70.f);

    window.on(FRAMEBUFFER_SIZE, [&](const TotoGL::VectorEvent& event) {
        glViewport(0, 0, int(event.x), int(event.y));
        camera.setPersective(FOV, (float)event.x / event.y, 1.f, 100.f);
    });
    window.on(MOUSE_BUTTON, [&](const TotoGL::InputEvent& event) {
        if (event.button != GLFW_MOUSE_BUTTON_1)
            return;
        holding = event.action;
    });
    window.on(CURSOR_POSITION, [&](const TotoGL::VectorEvent& event) {
        if (!holding)
            return;
        orbit.rotate(
            -event.dy / HEIGHT * 2 * FOV,
            -event.dx / HEIGHT * 2 * FOV);
    });
    window.on(SCROLL, [&](const TotoGL::VectorEvent& event) {
        orbit.distance() += event.y;
    });
    window.on(KEY, [&](const TotoGL::InputEvent& event) {
        if (event.button == GLFW_KEY_UP)
            velocity.z = -(event.action != GLFW_RELEASE);
        else if (event.button == GLFW_KEY_DOWN)
            velocity.z = (event.action != GLFW_RELEASE);
        if (event.button == GLFW_KEY_LEFT)
            velocity.x = -(event.action != GLFW_RELEASE);
        if (event.button == GLFW_KEY_RIGHT)
            velocity.x = (event.action != GLFW_RELEASE);
    });
}

TotoGL::RenderObject& makeObject() {
    static const auto tex_id = TotoGL::TextureFactory::create(
        // TotoGL::Texture(std::ifstream("assets/textures/kirby.png")));
        TotoGL::Texture(std::ifstream("assets/textures/earth.jpg")));
    static const auto mesh_id = TotoGL::MeshFactory::create(
        TotoGL::Mesh::sphere());
    static const auto mat_id = TotoGL::ShaderMaterialFactory::create(
        TotoGL::ShaderMaterial(
            std::ifstream("assets/shader/shader.vert"),
            std::ifstream("assets/shader/phong.frag")));

    static auto& texture = TotoGL::TextureFactory::get(tex_id);
    // static auto& mesh = TotoGL::MeshFactory::get(mesh_id);
    static auto& material = TotoGL::ShaderMaterialFactory::get(mat_id);

    material.uniform("u_texture", texture);

    const auto obj_id = TotoGL::RenderObjectFactory::create(TotoGL::RenderObject(mesh_id, mat_id));
    return TotoGL::RenderObjectFactory::get(obj_id);
}

TotoGL::RenderObject& makeHelper() {
    static const auto mesh_id = TotoGL::MeshFactory::create(
        TotoGL::Mesh::sphere());
    static const auto mat_id = TotoGL::ShaderMaterialFactory::create(
        TotoGL::ShaderMaterial(
            std::ifstream("assets/shader/shader.vert"),
            std::ifstream("assets/shader/uv.frag")));
    const auto obj_id = TotoGL::RenderObjectFactory::create(TotoGL::RenderObject(mesh_id, mat_id));
    return TotoGL::RenderObjectFactory::get(obj_id);
}
