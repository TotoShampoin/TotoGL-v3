// #include <TotoGL/TotoGL.hpp>

#include "TotoGL/CameraControl/OrbitControl.hpp"
#include "TotoGL/Primitives/Clock.hpp"
#include "TotoGL/RenderObject/Camera.hpp"
#include "TotoGL/RenderObject/RenderObject.hpp"
#include "TotoGL/RenderObject/ShaderMaterial.hpp"
#include "TotoGL/Renderer.hpp"
#include "TotoGL/Window.hpp"
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
        objects.push_back(makeObject());
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

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while (!window.shouldClose()) {
        float time = clock.getTime();
        float delta = clock.getDeltaTime();
        // auto [width, height] = window.size();

        material.uniform("u_time", time);
        material.uniform("u_projection", camera.projection());

        orbit.apply(camera);

        window.draw([&]() {
            glClearColor(.25, 0., .25, 1.);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            for (int i = 0; i < 6; i++) {
                auto& object = objects[i].get();
                auto modelview = camera.view() * object.transformMatrix();
                auto normal = glm::mat3(glm::transpose(glm::inverse(modelview)));
                object.rotation() += glm::vec3(1, 1, 1) * delta;

                material.uniform("u_modelview", modelview);
                material.uniform("u_normal", normal);
                objects[i].get().draw();
            }
        });
    }

    return 0;
}
