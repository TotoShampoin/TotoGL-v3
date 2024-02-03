#include <TotoGL/TotoGL.hpp>

#include <chrono>
#include <glm/ext/matrix_transform.hpp>
#include <glm/matrix.hpp>
#include <glm/trigonometric.hpp>
#include <vector>

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/glm.hpp>

int main(int argc, const char* argv[]) {
    using TotoGL::ShaderType::FRAGMENT;
    using TotoGL::ShaderType::VERTEX;
    using TotoGL::VectorEventName::FRAMEBUFFER_SIZE;

    using MaterialFactory = TotoGL::Factory<TotoGL::Material>;
    using MeshFactory = TotoGL::Factory<TotoGL::Mesh>;
    using TextureFactory = TotoGL::Factory<TotoGL::Texture>;
    using VertexShaderFactory = TotoGL::Factory<TotoGL::Shader<VERTEX>>;
    using FragmentShaderFactory = TotoGL::Factory<TotoGL::Shader<FRAGMENT>>;
    using RenderObjectFactory = TotoGL::Factory<TotoGL::RenderObject>;

    // auto& material_factory = TotoGL::Factory<TotoGL::Material>::get();
    // auto& mesh_factory = TotoGL::Factory<TotoGL::Mesh>::get();
    // auto& texture_factory = TotoGL::Factory<TotoGL::Texture>::get();
    // auto& vertex_shader_factory = TotoGL::Factory<TotoGL::Shader<VERTEX>>::get();
    // auto& fragment_shader_factory = TotoGL::Factory<TotoGL::Shader<FRAGMENT>>::get();
    // auto& render_object_factory = TotoGL::Factory<TotoGL::RenderObject>::get();

    // Variables
    auto window = TotoGL::Window(640, 480, "a title");
    auto renderer = TotoGL::Renderer();

    window.on(FRAMEBUFFER_SIZE, [](const TotoGL::VectorEvent& event) {
        glViewport(0, 0, int(event.x), int(event.y));
    });

    std::vector<TotoGL::VertexType> vertices = {
        { { -.5, .5, 0 }, { 0, 0, 1 }, { 0, 0 } },
        { { .5, .5, 0 }, { 0, 0, 1 }, { 1, 0 } },
        { { .5, -.5, 0 }, { 0, 0, 1 }, { 1, 1 } },
        { { -.5, -.5, 0 }, { 0, 0, 1 }, { 0, 1 } },
    };
    std::vector<uint> triangles = { 0, 1, 2, 0, 2, 3 };

    auto t1_id = TextureFactory::create(TotoGL::Texture(std::ifstream("assets/textures/logoIMAC.png")));
    auto t2_id = TextureFactory::create(TotoGL::Texture(std::ifstream("assets/textures/Apple_Computer_Logo_rainbow.svg.png")));
    auto mesh_id = MeshFactory::create(TotoGL::Mesh(vertices, triangles));
    auto mat_id = MaterialFactory::create(TotoGL::Material(std::ifstream("assets/shader/shader.vert"), std::ifstream("assets/shader/shader.frag")));

    auto& texture_1 = TextureFactory::get(t1_id);
    auto& texture_2 = TextureFactory::get(t2_id);
    auto& mesh = MeshFactory::get(mesh_id);
    auto& material = MaterialFactory::get(mat_id);

    auto obj_id = RenderObjectFactory::create(TotoGL::RenderObject(mesh, material));
    auto& object = RenderObjectFactory::get(obj_id);

    auto start = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock().now().time_since_epoch()).count();

    // Initialisation
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Traitement
    while (!window.shouldClose()) {
        auto now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock().now().time_since_epoch()).count();
        auto time = (now - start) / 1000.f;
        window.draw([&]() {
            glClearColor(.25, 0., .25, 1.);
            glClear(GL_COLOR_BUFFER_BIT);
            auto [width, height] = window.size();
            auto projection = glm::perspective(glm::radians(70.f), (float)width / height, .0001f, 100.f);
            auto modelview = glm::rotate(glm::translate(glm::mat4(1), { 0, 0, -2 }), time, { 1, 1, 0 });
            auto normal = glm::mat3(glm::transpose(glm::inverse(modelview)));

            material.uniform("u_time", time);
            material.uniform("u_texture", (int(std::floor(time)) % 2) ? texture_1 : texture_2);

            material.uniform("u_projection", projection);
            material.uniform("u_modelview", modelview);
            material.uniform("u_normal", normal);

            object.draw();
        });
    }

    return 0;
}
