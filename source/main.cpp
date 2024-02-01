#include "TotoGL/Primitives/RenderObject.hpp"
#include <TotoGL/TotoGL.hpp>

#include <GLFW/glfw3.h>

#include <chrono>
#include <cmath>
#include <string>
#include <vector>

int main(int argc, const char* argv[]) {
    using TotoGL::ShaderType::FRAGMENT;
    using TotoGL::ShaderType::VERTEX;

    // Variables
    auto window = TotoGL::Window(640, 480, "a title");
    auto renderer = TotoGL::Renderer();

    auto vobj = TotoGL::VertexObject();

    auto vertex = TotoGL::Shader(VERTEX);
    auto fragment = TotoGL::Shader(FRAGMENT);
    auto program = TotoGL::ShaderProgram();

    auto mesh = TotoGL::RenderObject(vobj, program);

    auto texture_1 = TotoGL::Texture();
    auto texture_2 = TotoGL::Texture();

    // Initialisation
    std::vector<TotoGL::VertexType> vertices = {
        { { -.5, .5, 0 }, { 0, 0, 0 }, { 0, 0 } },
        { { .5, .5, 0 }, { 0, 0, 0 }, { 1, 0 } },
        { { .5, -.5, 0 }, { 0, 0, 0 }, { 1, 1 } },
        { { -.5, -.5, 0 }, { 0, 0, 0 }, { 0, 1 } },
    };
    std::vector<uint> triangles = { 0, 1, 2, 0, 2, 3 };

    vobj.load(vertices, triangles);
    vertex.load(std::ifstream("assets/shader/shader.vert"));
    fragment.load(std::ifstream("assets/shader/shader.frag"));

    program
        .attach(vertex)
        .attach(fragment)
        .link();

    texture_1.load(std::ifstream("assets/textures/logoIMAC.png"));
    texture_2.load(std::ifstream("assets/textures/Apple_Computer_Logo_rainbow.svg.png"));

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
            program.uniform("u_time", time);
            if (int(std::floor(time)) % 2) {
                program.uniform("u_texture", texture_1);
            } else {
                program.uniform("u_texture", texture_2);
            }
            mesh.draw();
        });
    }

    return 0;
}
