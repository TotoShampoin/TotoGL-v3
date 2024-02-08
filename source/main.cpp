#include <TotoGL/TotoGL.hpp>

#include <glm/glm.hpp>

void event(
    TotoGL::Window& window,
    TotoGL::Camera& camera,
    bool& holding,
    TotoGL::OrbitControl& orbit,
    glm::vec3& velocity);

TotoGL::ShaderMaterialFactory::ObjectInstanceId getMaterial();
TotoGL::RenderObject& makeObject();

int main(int /* argc */, const char** /* argv */) {
    using TotoGL::ShaderType::FRAGMENT;
    using TotoGL::ShaderType::VERTEX;

    using TotoGL::InputEventName::KEY;
    using TotoGL::InputEventName::MOUSE_BUTTON;
    using TotoGL::VectorEventName::CURSOR_POSITION;
    using TotoGL::VectorEventName::FRAMEBUFFER_SIZE;
    using TotoGL::VectorEventName::SCROLL;

    constexpr int WIDTH = 640;
    constexpr int HEIGHT = 480;
    constexpr float FOV = glm::radians(70.f);

    auto window = TotoGL::Window(WIDTH, HEIGHT, "a title");
    auto renderer = TotoGL::Renderer();

    auto& material = TotoGL::ShaderMaterialFactory::get(getMaterial());

    auto objects = std::vector<std::reference_wrapper<TotoGL::RenderObject>>();

    for (int i = 0; i < 6; i++) {
        auto& obj = makeObject();
        obj.mesh().cull_face() = TotoGL::Mesh::CullFace::FRONT;
        obj.mesh().draw_method() = TotoGL::Mesh::DrawMethod::LINES;
        objects.push_back(obj);
    }

    objects[0].get().translate({ 0, 0, 2 });
    objects[1].get().translate({ 0, 0, -2 });
    objects[2].get().translate({ 0, 2, 0 });
    objects[3].get().translate({ 0, -2, 0 });
    objects[4].get().translate({ 2, 0, 0 });
    objects[5].get().translate({ -2, 0, 0 });

    auto camera = TotoGL::Camera::Perspective(FOV, (float)WIDTH / HEIGHT, .1f, 100.f);
    auto clock = TotoGL::Clock();

    auto orbit = TotoGL::OrbitControl(0, 0, 8);

    bool holding = false;
    glm::vec3 velocity = { 0, 0, 0 };

    event(window, camera, holding, orbit, velocity);

    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_FRONT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    renderer.clearColor({ 0, 0, 0, 1 });
    while (!window.shouldClose()) {
        float time = clock.getTime();
        float delta = clock.getDeltaTime();
        // auto [width, height] = window.size();

        material.uniform("u_time", time);

        orbit.apply(camera);

        window.draw([&]() {
            renderer.clear();
            for (int i = 0; i < 6; i++) {
                auto& object = objects[i].get();
                object.rotation() += glm::vec3(1, 1, 1) * delta;
                renderer.render(object, camera);
            }
        });
    }

    return 0;
}
