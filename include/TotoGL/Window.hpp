#pragma once

#include <GLFW/glfw3.h>
#include <functional>
#include <stdexcept>
#include <string>

namespace TotoGL {

class Window {
public:
    Window(const int& width, const int& height, const std::basic_string<char>& title) {
        init();
        _glfw_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
        if (!_glfw_window) {
            const char* error;
            glfwGetError(&error);
            throw std::runtime_error(error);
        }
        glfwMakeContextCurrent(_glfw_window);
    }
    Window(const Window&) = delete;
    ~Window() {
        glfwDestroyWindow(_glfw_window);
    }

    void draw(const std::function<void(void)>& callback) {
        glfwMakeContextCurrent(_glfw_window);
        callback();
        glfwSwapBuffers(_glfw_window);
        glfwPollEvents();
    }

    bool shouldClose() { return glfwWindowShouldClose(_glfw_window); }

    const GLFWwindow* glfwWindow() const { return _glfw_window; }

private:
    GLFWwindow* _glfw_window;

    static void init() {
        static bool is_glfw_init = false;
        if (is_glfw_init)
            return;
        if (glfwInit())
            return;
        const char* error;
        glfwGetError(&error);
        throw std::runtime_error(error);
    }
};

} // namespace TotoGL
