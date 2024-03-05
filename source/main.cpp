#include <TotoGL/TotoGL.hpp>

#include <TotoGL/RenderObject/Light.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>

void event(
    TotoGL::Window& window,
    TotoGL::Camera& camera,
    bool& holding,
    TotoGL::OrbitControl& orbit,
    glm::vec3& velocity);

TotoGL::ShaderMaterialFactory::ObjectInstanceId getMaterial();
TotoGL::RenderObject& makeObject();
TotoGL::RenderObject& makeHelper();

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

    auto kirbies = std::vector<std::reference_wrapper<TotoGL::RenderObject>>();

    for (int i = 0; i < 6; i++) {
        auto& kirby = makeObject();
        kirbies.push_back(kirby);
    }

    kirbies[0].get().translate({ 0, 0, 2 });
    kirbies[1].get().translate({ 0, 0, -2 });
    kirbies[2].get().translate({ 0, 2, 0 });
    kirbies[3].get().translate({ 0, -2, 0 });
    kirbies[4].get().translate({ 2, 0, 0 });
    kirbies[5].get().translate({ -2, 0, 0 });

    auto lights = std::vector<TotoGL::Light>();
    {
        auto ambient = TotoGL::Light(glm::vec3(1, 1, 1), .25, TotoGL::LightType::AMBIENT);
        auto dirlight = TotoGL::Light(glm::vec3(1, 1, 1), 1, TotoGL::LightType::DIRECTIONAL);
        dirlight.position() = { 3, 3, 3 };
        dirlight.transformation().lookAt({ 0, 0, 0 });

        lights.push_back(ambient);
        lights.push_back(dirlight);
    }
    auto light_helper = makeHelper();

    auto& material = kirbies[0].get().material();

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

    // material.uniform("u_light.color", ambient.color());
    // material.uniform("u_light.strength", ambient.strength());
    // material.uniform("u_light[0].color", light.color());
    // material.uniform("u_light[0].strength", light.strength());
    // material.uniform("u_light[0].type", static_cast<int>(light.type()));

    renderer.clearColor({ 0, 0, 0, 1 });
    while (!window.shouldClose()) {
        float time = clock.getTime();
        float delta = clock.getDeltaTime();
        // auto [width, height] = window.size();

        orbit.apply(camera);

        // light.rotate(delta, { 0, 1, 0 });
        // light_helper.rotate(delta, { 0, 1, 0 });

        material.uniform("u_time", time);

        // material.uniform("u_light[0].pos_or_dir",
        //     glm::mat3(glm::transpose(glm::inverse(camera.view()))) * light.direction());
        // material.uniform("u_light.pos_or_dir",
        //     glm::vec3(camera.view() * glm::vec4(point_light.position(), 1)));

        window.draw([&]() {
            renderer.clear();
            for (int i = 0; i < 6; i++) {
                auto& object = kirbies[i].get();
                object.rotation() += glm::vec3(2, 3, 5) * delta;
                renderer.render(object, camera, lights);
            }
            light_helper.position() = lights[1].position();
            light_helper.rotation() = lights[1].transformation().rotation();

            renderer.render(light_helper, camera);
        });
    }

    return 0;
}
