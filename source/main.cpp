#include <TotoGL/TotoGL.hpp>

#include <chrono>
#include <cmath>
#include <vector>

int main(int argc, const char* argv[]) {
    using TotoGL::ShaderType::FRAGMENT;
    using TotoGL::ShaderType::VERTEX;
    using TotoGL::VectorEventName::FRAMEBUFFER_SIZE;

    auto& material_factory = TotoGL::Factory<TotoGL::Material>::get();
    auto& mesh_factory = TotoGL::Factory<TotoGL::Mesh>::get();
    auto& texture_factory = TotoGL::Factory<TotoGL::Texture>::get();
    auto& vertex_shader_factory = TotoGL::Factory<TotoGL::Shader<VERTEX>>::get();
    auto& fragment_shader_factory = TotoGL::Factory<TotoGL::Shader<FRAGMENT>>::get();
    auto& render_object_factory = TotoGL::Factory<TotoGL::RenderObject>::get();

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

    auto t1_id = texture_factory.createInstance(TotoGL::Texture(std::ifstream("assets/textures/logoIMAC.png")));
    auto t2_id = texture_factory.createInstance(TotoGL::Texture(std::ifstream("assets/textures/Apple_Computer_Logo_rainbow.svg.png")));
    auto mesh_id = mesh_factory.createInstance(TotoGL::Mesh(vertices, triangles));
    auto mat_id = material_factory.createInstance(TotoGL::Material(std::ifstream("assets/shader/shader.vert"), std::ifstream("assets/shader/shader.frag")));

    auto& texture_1 = texture_factory.getInstance(t1_id);
    auto& texture_2 = texture_factory.getInstance(t2_id);
    auto& mesh = mesh_factory.getInstance(mesh_id);
    auto& material = material_factory.getInstance(mat_id);

    auto obj_id = render_object_factory.createInstance(TotoGL::RenderObject(mesh, material));
    auto& object = render_object_factory.getInstance(obj_id);

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
