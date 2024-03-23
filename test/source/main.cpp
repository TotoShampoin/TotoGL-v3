#include "TotoGL/Primitives/BufferTexture.hpp"
#include "TotoGL/Primitives/Shader.hpp"
#include "TotoGL/RenderObject/Mesh.hpp"
#include "TotoGL/RenderObject/RenderObject.hpp"
#include "TotoGL/RenderObject/Scene.hpp"
#include "TotoGL/RenderObject/ShaderMaterial.hpp"
#include "TotoGL/Window.hpp"
#include <TotoGL/TotoGL.hpp>
#include <fstream>

int main(int /* argc */, const char** /* argv */) {
    using TotoGL::ShaderType::FRAGMENT;
    using TotoGL::ShaderType::VERTEX;

    using TotoGL::InputEventName::KEY;
    using TotoGL::InputEventName::MOUSE_BUTTON;
    using TotoGL::VectorEventName::CURSOR_POSITION;
    using TotoGL::VectorEventName::FRAMEBUFFER_SIZE;
    using TotoGL::VectorEventName::SCROLL;

    constexpr int WIDTH = 960;
    constexpr int HEIGHT = 720;

    int width = WIDTH;
    int height = HEIGHT;
    float fov = glm::radians(70.f);

    auto window = TotoGL::Window(width, height, "a title");
    auto renderer = TotoGL::Renderer();
    auto camera = TotoGL::Camera::Perspective(fov, static_cast<float>(width) / height, 0.1f, 4.f);
    auto clock = TotoGL::Clock();

    auto scene = TotoGL::SceneFactory::create();

    auto control = TotoGL::FreeUseControl();
    control.bindEvents(window);

    auto sky = TotoGL::SkydomeFactory::create(
        TotoGL::Skydome(
            *TotoGL::TextureFactory::create(
                TotoGL::Texture(std::ifstream("assets/textures/skydome.jpg")))));

    auto light = TotoGL::LightFactory::create(
        TotoGL::Light({ 1, 1, 1 }, 1, TotoGL::LightType::DIRECTIONAL));
    light->setDirection({ 1, -1, 1 });

    auto plane = TotoGL::RenderObjectFactory::create(
        TotoGL::RenderObject(
            TotoGL::MeshFactory::create(
                TotoGL::Mesh::quad()),
            TotoGL::ShaderMaterialFactory::create(
                TotoGL::ShaderMaterial(
                    TotoGL::VertexShader(std::ifstream("assets/shaders/shader.vert")),
                    TotoGL::FragmentShader(std::ifstream("assets/shaders/shader.frag"))))));

    auto kirby = TotoGL::RenderObjectFactory::create(
        TotoGL::RenderObject(
            TotoGL::MeshFactory::create(
                TotoGL::Mesh::sphere(1, 32, 32)),
            TotoGL::ShaderMaterialFactory::create(
                TotoGL::ShaderMaterial(
                    TotoGL::VertexShader(std::ifstream("assets/shaders/shader.vert")),
                    TotoGL::FragmentShader(std::ifstream("assets/shaders/phong.frag"))))));

    auto override_shader = TotoGL::ShaderMaterialFactory::create(
        TotoGL::ShaderMaterial(
            TotoGL::VertexShader(std::ifstream("assets/shaders/shader.vert")),
            TotoGL::FragmentShader(std::ifstream("assets/shaders/depth.frag"))));

    auto kirby_texture = TotoGL::TextureFactory::create(
        TotoGL::Texture(std::ifstream("assets/textures/kirby.png")));

    auto render_texture = TotoGL::BufferTextureFactory::create(
        TotoGL::BufferTexture(WIDTH, HEIGHT));
    auto render_texture_2 = TotoGL::BufferTextureFactory::create(
        TotoGL::BufferTexture(WIDTH, HEIGHT));

    plane->position() = { 0, 0, -2 };
    plane->scaling() = { static_cast<float>(WIDTH) / HEIGHT, -1, 1 };
    plane->mesh().cull_face() = TotoGL::Mesh::CullFace::BACK;

    kirby->position() = { 2, 0, 0 };
    kirby->material().uniform("u_texture", kirby_texture);
    plane->material().uniform("u_texture", render_texture_2->texture());

    camera.position() = { 3, 1, 2 };
    camera.lookAt({ 0, 0, 0 });

    scene->add(sky);
    scene->add(light);
    scene->add(plane);
    scene->add(kirby);

    window.on(FRAMEBUFFER_SIZE, [&](const TotoGL::VectorEvent& e) {
        width = e.x;
        height = e.y;
    });

    while (!window.shouldClose()) {
        auto time = clock.getTime();
        auto delta_time = clock.getDeltaTime();

        control.update(delta_time);
        control.apply(camera);

        kirby->lookAt(camera.position());
        kirby->rotate(-glm::pi<float>() / 2, camera.transformation().rotationMatrix()[1]);

        render_texture->draw([&]() {
            // camera.position() = { 3, 1, 2 };
            // camera.lookAt({ 0, 0, 0 });
            camera.setPersective(fov, static_cast<float>(WIDTH) / HEIGHT, 0.1f, 5.f);
            renderer.clear();
            // renderer.render(scene, camera);
            renderer.renderOverrideMaterial(*scene, camera, *override_shader);
        });
        render_texture_2->copy(*render_texture);

        window.draw([&]() {
            camera.setPersective(fov, static_cast<float>(width) / height, 0.1f, 10.f);
            renderer.clear();
            renderer.render(*scene, camera);
        });
    }

    return 0;
}
