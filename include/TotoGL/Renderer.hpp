#pragma once

#include <GL/glew.h>
#include <stdexcept>

namespace TotoGL {

class Renderer {
public:
    Renderer() {
        init();
    }
    Renderer(const Renderer&) = delete;
    ~Renderer() { }

    void render() { }

private:
    void init() {
        static bool is_glew_init = false;
        if (is_glew_init)
            return;
        if (auto error = glewInit()) {
            throw std::runtime_error(reinterpret_cast<const char*>(glewGetErrorString(error)));
        }
    }
};

} // namespace TotoGL
