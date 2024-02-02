#include <TotoGL/TotoGL.hpp>

#include <GLFW/glfw3.h>

#include <chrono>
#include <cmath>
#include <vector>

int main(int argc, const char* argv[]) {
    using TotoGL::ShaderType::FRAGMENT;
    using TotoGL::ShaderType::VERTEX;
    using TotoGL::VectorEventName::FRAMEBUFFER_SIZE;

    // Variables
    auto window = TotoGL::Window(640, 480, "a title");
    auto renderer = TotoGL::Renderer();

    window.on(FRAMEBUFFER_SIZE, [](const TotoGL::VectorEvent& event) {
        glViewport(0, 0, int(event.x), int(event.y));
    });

    std::vector<TotoGL::VertexType> vertices = {
        { { -.5, .5, 0 }, { 0, 0, 0 }, { 0, 0 } },
        { { .5, .5, 0 }, { 0, 0, 0 }, { 1, 0 } },
        { { .5, -.5, 0 }, { 0, 0, 0 }, { 1, 1 } },
        { { -.5, -.5, 0 }, { 0, 0, 0 }, { 0, 1 } },
    };
    std::vector<uint> triangles = { 0, 1, 2, 0, 2, 3 };

    auto mesh = TotoGL::VertexObject(vertices, triangles);
    auto material = TotoGL::Material(
        TotoGL::Shader<VERTEX>(std::ifstream("assets/shader/shader.vert")),
        TotoGL::Shader<FRAGMENT>(std::ifstream("assets/shader/shader.frag")));

    auto object = TotoGL::RenderObject(mesh, material);

    auto texture_1 = TotoGL::Texture(std::ifstream("assets/textures/logoIMAC.png"));
    auto texture_2 = TotoGL::Texture(std::ifstream("assets/textures/Apple_Computer_Logo_rainbow.svg.png"));

    // Initialisation
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    auto start = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock().now().time_since_epoch()).count();

    // Traitement
    while (!window.shouldClose()) {
        auto now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock().now().time_since_epoch()).count();
        auto time = (now - start) / 1000.f;
        window.draw([&]() {
            glClearColor(0., 0., .5, 1.);
            glClear(GL_COLOR_BUFFER_BIT);
            material.uniform("u_time", time);
            if (int(std::floor(time)) % 2) {
                material.uniform("u_texture", texture_1);
            } else {
                material.uniform("u_texture", texture_2);
            }
            object.draw();
        });
    }

    return 0;
}
