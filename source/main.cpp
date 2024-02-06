#include <TotoGL/TotoGL.hpp>

#include <glm/glm.hpp>

int main(int argc, const char* argv[]) {
    using TotoGL::ShaderType::FRAGMENT;
    using TotoGL::ShaderType::VERTEX;

    using TotoGL::InputEventName::KEY;
    using TotoGL::InputEventName::MOUSE_BUTTON;
    using TotoGL::VectorEventName::CURSOR_POSITION;
    using TotoGL::VectorEventName::FRAMEBUFFER_SIZE;
    using TotoGL::VectorEventName::SCROLL;

    int width = 640;
    int height = 480;
    float fov = glm::radians(70.f);

    auto window = TotoGL::Window(width, height, "a title");
    auto renderer = TotoGL::Renderer();

    const auto tex_id = TotoGL::TextureFactory::create(TotoGL::Texture(std::ifstream("assets/textures/XYZ.png")));
    const auto mat_id = TotoGL::ShaderMaterialFactory::create(TotoGL::ShaderMaterial(std::ifstream("assets/shader/shader.vert"), std::ifstream("assets/shader/shader.frag")));
    const auto mesh_id = TotoGL::MeshFactory::create(TotoGL::Mesh::cube());
    const auto obj_id = TotoGL::RenderObjectFactory::create(TotoGL::RenderObject(mesh_id, mat_id));

    auto& texture = TotoGL::TextureFactory::get(tex_id);
    auto& mesh = TotoGL::MeshFactory::get(mesh_id);
    auto& material = TotoGL::ShaderMaterialFactory::get(mat_id);
    auto& object = TotoGL::RenderObjectFactory::get(obj_id);

    auto camera = TotoGL::Camera::Perspective(fov, (float)width / height, .1f, 100.f);
    auto clock = TotoGL::Clock();

    auto orbit = TotoGL::OrbitControl(0, 0, 4);
    // auto freefly = TotoGL::FreeflyControl(0, 0);

    bool holding = false;
    glm::vec3 velocity = { 0, 0, 0 };

    object.translate({ 0, 0, 2 });

    // freefly.position() = { 0, 1, 0 };

    window.on(FRAMEBUFFER_SIZE, [&](const TotoGL::VectorEvent& event) {
        width = event.x;
        height = event.y;
        glViewport(0, 0, int(event.x), int(event.y));
        camera.setPersective(fov, (float)event.x / event.y, 1.f, 100.f);
    });
    window.on(MOUSE_BUTTON, [&](const TotoGL::InputEvent& event) {
        if (event.button != GLFW_MOUSE_BUTTON_1)
            return;
        holding = event.action;
    });
    window.on(CURSOR_POSITION, [&](const TotoGL::VectorEvent& event) {
        if (!holding)
            return;
        orbit.rotate(
            -event.dy / height * 2 * fov,
            -event.dx / height * 2 * fov);
        // freefly.rotate(
        //     -event.dx / height * fov,
        //     -event.dy / height * fov);
    });
    window.on(SCROLL, [&](const TotoGL::VectorEvent& event) {
        orbit.distance() += event.y;
    });
    window.on(KEY, [&](const TotoGL::InputEvent& event) {
        if (event.button == GLFW_KEY_UP)
            velocity.z = -(event.action != GLFW_RELEASE);
        else if (event.button == GLFW_KEY_DOWN)
            velocity.z = (event.action != GLFW_RELEASE);
        if (event.button == GLFW_KEY_LEFT)
            velocity.x = -(event.action != GLFW_RELEASE);
        if (event.button == GLFW_KEY_RIGHT)
            velocity.x = (event.action != GLFW_RELEASE);
    });

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while (!window.shouldClose()) {
        float time = clock.getTime();
        float delta = clock.getDeltaTime();

        // object.rotate(delta, { 1, 1, 0 });

        auto [width, height] = window.size();

        orbit.apply(camera);
        orbit.position() += velocity * delta;
        // freefly.apply(camera);
        // freefly.moveForward(velocity.z * delta);

        auto modelview = camera.view() * object.transformMatrix();
        auto normal = glm::mat3(glm::transpose(glm::inverse(modelview)));

        material.uniform("u_time", time);
        material.uniform("u_texture", texture);
        material.uniform("u_projection", camera.projection());
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
