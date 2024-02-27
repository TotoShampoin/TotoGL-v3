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

    auto ambient = TotoGL::Light(glm::vec3(1, 1, 1), .25, TotoGL::LightType::AMBIENT);
    auto dir_light = TotoGL::Light(glm::vec3(1, 1, 1), 1., TotoGL::LightType::DIRECTIONAL);
    dir_light.position() = { 0, 3, 5 };
    dir_light.transformation().lookAt({ 0, 0, 0 });

    {
        auto& light_helper = makeHelper();
        light_helper.scaling() = { .5, .5, .5 };
        light_helper.position() = dir_light.position();
        light_helper.rotation() = dir_light.direction();
        kirbies.push_back(light_helper);
    }

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

    material.uniform("u_amb_light.color", ambient.color());
    material.uniform("u_amb_light.strength", ambient.strength());
    material.uniform("u_dir_light.color", dir_light.color());
    material.uniform("u_dir_light.strength", dir_light.strength());

    renderer.clearColor({ 0, 0, 0, 1 });
    while (!window.shouldClose()) {
        float time = clock.getTime();
        float delta = clock.getDeltaTime();
        // auto [width, height] = window.size();

        orbit.apply(camera);

        material.uniform("u_time", time);

        material.uniform("u_dir_light.direction",
            glm::mat3(glm::transpose(glm::inverse(camera.view()))) * dir_light.direction());

        window.draw([&]() {
            renderer.clear();
            for (int i = 0; i < 6; i++) {
                auto& object = kirbies[i].get();
                object.rotation() += glm::vec3(1, 1, 1) * delta;
                renderer.render(object, camera);
            }
            renderer.render(kirbies[6].get(), camera);
        });
    }

    return 0;
}
