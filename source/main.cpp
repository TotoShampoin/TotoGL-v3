#include <TotoGL/TotoGL.hpp>

#include "TotoGL/CameraControl/TrackballControl.hpp"
#include "TotoGL/Window.hpp"

#include <glm/glm.hpp>

int main(int argc, const char* argv[]) {
    using TotoGL::ShaderType::FRAGMENT;
    using TotoGL::ShaderType::VERTEX;

    using TotoGL::InputEventName::MOUSE_BUTTON;
    using TotoGL::VectorEventName::CURSOR_POSITION;
    using TotoGL::VectorEventName::FRAMEBUFFER_SIZE;

    int width = 640;
    int height = 480;

    auto window = TotoGL::Window(width, height, "a title");
    auto renderer = TotoGL::Renderer();

    const auto tex_id = TotoGL::TextureFactory::create(TotoGL::Texture(std::ifstream("assets/textures/XYZ.png")));
    const auto mat_id = TotoGL::MaterialFactory::create(TotoGL::Material(std::ifstream("assets/shader/shader.vert"), std::ifstream("assets/shader/shader.frag")));
    const auto mesh_id = TotoGL::MeshFactory::create(TotoGL::Mesh::cube());
    const auto robj_id = TotoGL::RenderObjectFactory::create(TotoGL::RenderObject(mesh_id, mat_id));

    auto& texture = TotoGL::TextureFactory::get(tex_id);
    auto& mesh = TotoGL::MeshFactory::get(mesh_id);
    auto& material = TotoGL::MaterialFactory::get(mat_id);
    auto& object = TotoGL::RenderObjectFactory::get(robj_id);

    auto camera = TotoGL::Camera::Perspective(glm::radians(70.f), (float)width / height, 1.f, 100.f);
    auto trackball = TotoGL::TrackballControl(0, 0, 2);
    auto clock = TotoGL::Clock();

    bool holding = false;

    // object.translate({ 0, 0, -2 });

    window.on(FRAMEBUFFER_SIZE, [&](const TotoGL::VectorEvent& event) {
        width = event.x;
        height = event.y;
        glViewport(0, 0, int(event.x), int(event.y));
        camera.setPersective(glm::radians(70.f), (float)event.x / event.y, 1.f, 100.f);
        material.uniform("u_projection", camera.projection());
    });
    window.on(MOUSE_BUTTON, [&](const TotoGL::InputEvent& event) {
        if (event.button != GLFW_MOUSE_BUTTON_1)
            return;
        holding = event.action;
    });
    window.on(CURSOR_POSITION, [&](const TotoGL::VectorEvent& event) {
        if (!holding)
            return;
        trackball.move(
            event.dy / width * glm::pi<float>(),
            event.dx / height * glm::pi<float>());
    });
    material.uniform("u_projection", camera.projection());
    material.uniform("u_texture", texture);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while (!window.shouldClose()) {
        float time = clock.getTime();
        float delta = clock.getDeltaTime();

        // object.rotate(delta, { 1, 1, 0 });

        auto [width, height] = window.size();

        trackball.apply(camera);

        auto modelview = camera.view() * object.transformMatrix();
        auto normal = glm::mat3(glm::transpose(glm::inverse(modelview)));

        material.uniform("u_time", time);

        material.uniform("u_modelview", modelview);
        material.uniform("u_normal", normal);

        window.draw([&]() {
            glClearColor(.25, 0., .25, 1.);
            glClear(GL_COLOR_BUFFER_BIT);

            object.draw();
        });
    }

    return 0;
}
