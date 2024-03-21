#include "TotoGL/GPUPointer/GPUPointer.hpp"
#include "TotoGL/GPUPointer/Texture.hpp"
#include "TotoGL/Primitives/Shader.hpp"
#include "TotoGL/RenderObject/Mesh.hpp"
#include "TotoGL/RenderObject/RenderObject.hpp"
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
    auto scene = TotoGL::Scene();
    auto camera = TotoGL::Camera::Perspective(fov, static_cast<float>(width) / height, 0.1f, 1000.f);
    auto clock = TotoGL::Clock();

    auto control = TotoGL::FreeUseControl();
    control.bindEvents(window);

    auto sky = TotoGL::SkydomeFactory::create(
        TotoGL::Skydome(
            *TotoGL::TextureFactory::create(
                TotoGL::Texture(std::ifstream("assets/textures/skydome.jpg")))));

    auto light = TotoGL::LightFactory::create(
        TotoGL::Light({ 1, 1, 1 }, 1, TotoGL::LightType::DIRECTIONAL));
    light->direction() = glm::normalize(glm::vec3 { 1, -1, 1 });

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
                    TotoGL::FragmentShader(std::ifstream("assets/shaders/shader.frag"))))));

    auto kirby_texture = TotoGL::TextureFactory::create(
        TotoGL::Texture(std::ifstream("assets/textures/kirby.png")));

    /* Frame buffer test */
    auto render_texture = TotoGL::TextureFactory::create(
        TotoGL::Texture());

    render_texture->bind();
    auto framebuffer = TotoGL::FrameBufferId();
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.id());

    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, HEIGHT, 0, GL_RGBA,
        GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    auto depth_buffer = TotoGL::RenderBufferId();
    glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer.id());
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WIDTH, HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_buffer.id());

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, render_texture->texId().id(), 0);
    GLenum draw_buffers[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, draw_buffers);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Framebuffer is not complete!" << std::endl;
    }
    /* */

    plane->scaling() = { static_cast<float>(WIDTH) / HEIGHT, -1, 1 };
    plane->mesh().cull_face() = TotoGL::Mesh::CullFace::BACK;

    kirby->position() = { 2, 0, 0 };
    kirby->material().uniform("u_texture", kirby_texture);

    plane->material().uniform("u_texture", render_texture);

    camera.position() = { 3, 1, 2 };
    camera.lookAt({ 0, 0, 0 });

    scene.add(sky);
    scene.add(light);
    scene.add(plane);
    scene.add(kirby);

    renderer.clearColor({ 0, 0, 0, 1 });

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.id());
    glViewport(0, 0, WIDTH, HEIGHT);
    renderer.clear();
    renderer.render(scene, camera);

    window.on(FRAMEBUFFER_SIZE, [&](const TotoGL::VectorEvent& e) {
        width = e.x;
        height = e.y;
        camera.setPersective(fov, static_cast<float>(width) / height, 0.1f, 1000.f);
    });

    window.on(KEY, [&](const TotoGL::InputEvent& e) {
        if (e.action && e.button == GLFW_KEY_R) {
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.id());
            glViewport(0, 0, WIDTH, HEIGHT);
            plane->material().uniform("u_texture", kirby_texture);
            renderer.clear();
            renderer.render(scene, camera);
        }
    });

    while (!window.shouldClose()) {
        auto delta_time = clock.getDeltaTime();
        control.update(delta_time);
        control.apply(camera);

        kirby->lookAt(camera.position());
        kirby->rotate(-glm::pi<float>() / 2, camera.transformation().rotationMatrix()[1]);

        window.draw([&]() {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glViewport(0, 0, width, height);
            plane->material().uniform("u_texture", render_texture);
            renderer.clear();
            renderer.render(scene, camera);
        });
    }

    return 0;
}
