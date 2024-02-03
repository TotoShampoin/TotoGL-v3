#include <TotoGL/TotoGL.hpp>

#include <glm/glm.hpp>
#include <vector>

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

    constexpr int WIDTH = 640;
    constexpr int HEIGHT = 480;

    auto window
        = TotoGL::Window(WIDTH, HEIGHT, "a title");
    auto renderer = TotoGL::Renderer();

    std::vector<TotoGL::VertexType> vertices = {
        { { -.5, .5, 0 }, { 0, 0, 1 }, { 0, 0 } },
        { { .5, .5, 0 }, { 0, 0, 1 }, { 1, 0 } },
        { { .5, -.5, 0 }, { 0, 0, 1 }, { 1, 1 } },
        { { -.5, -.5, 0 }, { 0, 0, 1 }, { 0, 1 } },
    };
    std::vector<uint> triangles = { 0, 1, 2, 0, 2, 3 };

    auto tex_id = TextureFactory::create(TotoGL::Texture(std::ifstream("assets/textures/XYZ.png")));
    auto mat_id = MaterialFactory::create(TotoGL::Material(std::ifstream("assets/shader/shader.vert"), std::ifstream("assets/shader/shader.frag")));
    auto mesh_id = MeshFactory::create(TotoGL::Mesh(vertices, triangles));

    auto& texture = TextureFactory::get(tex_id);
    auto& mesh = MeshFactory::get(mesh_id);
    auto& material = MaterialFactory::get(mat_id);

    auto obj_id = RenderObjectFactory::create(TotoGL::RenderObject(mesh, material));
    auto& object = RenderObjectFactory::get(obj_id);

    auto camera = TotoGL::Camera::Perspective(glm::radians(70.f), (float)WIDTH / HEIGHT, 1.f, 100.f);
    auto clock = TotoGL::Clock();
    auto transform = TotoGL::Transformation();
    transform.translate({ 0, 0, -2 });

    // WHY IS THIS ONE NOT WORKING
    auto projection = camera.projection();
    material.uniform("u_projection", projection);

    window.on(FRAMEBUFFER_SIZE, [&](const TotoGL::VectorEvent& event) {
        glViewport(0, 0, int(event.x), int(event.y));
        camera.setPersective(glm::radians(70.f), (float)event.x / event.y, 1.f, 100.f);

        auto projection = camera.projection();
        material.uniform("u_projection", projection);
    });

    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_FRONT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while (!window.shouldClose()) {
        float time = clock.getTime();
        float delta = clock.getDeltaTime();

        transform.rotate(delta, { 1, 1, 0 });

        auto [width, height] = window.size();

        auto modelview = camera.view() * transform.matrix();
        auto normal = glm::mat3(glm::transpose(glm::inverse(modelview)));

        material.uniform("u_time", time);
        material.uniform("u_texture", texture);

        material.uniform("u_modelview", modelview);
        material.uniform("u_normal", normal);

        window.draw([&]() {
            glClearColor(.25, 0., .25, 1.);
            glClear(GL_COLOR_BUFFER_BIT);

            object.draw();
        });
    }

    return 0;
}
