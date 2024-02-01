#include "TotoGL/Primitives/RenderObject.hpp"
#include <TotoGL/TotoGL.hpp>

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

    // Initialisation
    std::vector<TotoGL::VertexType> vertices = {
        { { -.5, -.5, 0 }, { 0, 0, 0 }, { 0, 0 } },
        { { .5, -.5, 0 }, { 0, 0, 0 }, { 1, 0 } },
        { { .5, .5, 0 }, { 0, 0, 0 }, { 1, 1 } },
        { { -.5, .5, 0 }, { 0, 0, 0 }, { 0, 1 } },
    };
    std::vector<uint> triangles = { 0, 1, 2, 0, 2, 3 };

    vobj.load(vertices, triangles);
    vertex.load(std::ifstream("assets/shader/shader.vert"));
    fragment.load(std::ifstream("assets/shader/shader.frag"));

    program
        .attach(vertex)
        .attach(fragment)
        .link();

    // Traitement
    while (!window.shouldClose()) {
        window.draw([&]() {
            glClearColor(0., 0., 1., 1.);
            glClear(GL_COLOR_BUFFER_BIT);
            mesh.draw();
        });
    }

    return 0;
}
