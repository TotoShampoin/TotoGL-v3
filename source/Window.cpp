#include "TotoGL/Window.hpp"

#include "TotoGL/Misc/EventListener.hpp"
#include <GLFW/glfw3.h>
#include <functional>
#include <stdexcept>
#include <string>

namespace TotoGL {

Window::Window(const int& width, const int& height, const std::basic_string<char>& title) {
    init();
    _glfw_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (!_glfw_window) {
        const char* error;
        glfwGetError(&error);
        throw std::runtime_error(error);
    }
    bindEvents();
}

Window::~Window() {
    glfwDestroyWindow(_glfw_window);
}

void Window::init() {
    static bool is_glfw_init = false;
    if (is_glfw_init)
        return;
    if (glfwInit())
        return;
    const char* error;
    glfwGetError(&error);
    throw std::runtime_error(error);
}

void Window::draw(const std::function<void(void)>& callback) {
    glfwMakeContextCurrent(_glfw_window);
    callback();
    glfwSwapBuffers(_glfw_window);
    glfwPollEvents();
}

void Window::makeContextCurrent() {
    glfwMakeContextCurrent(_glfw_window);
}
bool Window::shouldClose() {
    return glfwWindowShouldClose(_glfw_window);
}
std::array<int, 2> Window::size() {
    std::array<int, 2> s;
    glfwGetWindowSize(_glfw_window, &s[0], &s[1]);
    return s;
}

// ? I want to reduce code redundancy here :(
void Window::on(const FlagEventName& name, const std::function<void(FlagEvent)>& callback) {
    _flag_event_listener.on(name, callback);
}
void Window::emit(const FlagEventName& name, const FlagEvent& value) {
    _flag_event_listener.emit(name, value);
}
void Window::on(const VectorEventName& name, const std::function<void(VectorEvent)>& callback) {
    _vector_event_listener.on(name, callback);
}
void Window::emit(const VectorEventName& name, const VectorEvent& value) {
    _vector_event_listener.emit(name, value);
}
void Window::on(const InputEventName& name, const std::function<void(InputEvent)>& callback) {
    _input_event_listener.on(name, callback);
}
void Window::emit(const InputEventName& name, const InputEvent& value) {
    _input_event_listener.emit(name, value);
}
void Window::bindEvents() {
    glfwMakeContextCurrent(_glfw_window);
    glfwSetWindowUserPointer(_glfw_window, this);
    glfwSetWindowCloseCallback(_glfw_window, [](GLFWwindow* glwin) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glwin));
        window->emit(FlagEventName::WINDOW_CLOSE, FlagEvent { true });
    });
    glfwSetWindowIconifyCallback(_glfw_window, [](GLFWwindow* glwin, int iconified) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glwin));
        window->emit(FlagEventName::WINDOW_ICONIFY, FlagEvent { (bool)iconified });
    });
    glfwSetWindowMaximizeCallback(_glfw_window, [](GLFWwindow* glwin, int maximized) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glwin));
        window->emit(FlagEventName::WINDOW_MAXIMIZE, FlagEvent { (bool)maximized });
    });
    glfwSetWindowFocusCallback(_glfw_window, [](GLFWwindow* glwin, int focused) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glwin));
        window->emit(FlagEventName::WINDOW_FOCUS, FlagEvent { (bool)focused });
    });
    glfwSetWindowRefreshCallback(_glfw_window, [](GLFWwindow* glwin) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glwin));
        window->emit(FlagEventName::WINDOW_REFRESH, FlagEvent { true });
    });

    glfwSetWindowSizeCallback(_glfw_window, [](GLFWwindow* glwin, int width, int height) {
        static int last_width = width, last_height = height;
        int delta_width = width - last_width, delta_height = height - last_height;
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glwin));
        window->emit(VectorEventName::WINDOW_SIZE, VectorEvent { (double)width, (double)height, (double)delta_width, (double)delta_height });
        last_width = width;
        last_height = height;
    });
    glfwSetFramebufferSizeCallback(_glfw_window, [](GLFWwindow* glwin, int width, int height) {
        static int last_width = width, last_height = height;
        int delta_width = width - last_width, delta_height = height - last_height;
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glwin));
        window->emit(VectorEventName::FRAMEBUFFER_SIZE, VectorEvent { (double)width, (double)height, (double)delta_width, (double)delta_height });
        last_width = width;
        last_height = height;
    });
    glfwSetWindowContentScaleCallback(_glfw_window, [](GLFWwindow* glwin, float xscale, float yscale) {
        static float last_xscale = xscale, last_yscale = yscale;
        float delta_xscale = xscale - last_xscale, delta_yscale = yscale - last_yscale;
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glwin));
        window->emit(VectorEventName::CONTENT_SCALE_SIZE, VectorEvent { (double)xscale, (double)yscale, delta_xscale, delta_yscale });
        last_xscale = xscale;
        last_yscale = yscale;
    });
    glfwSetWindowPosCallback(_glfw_window, [](GLFWwindow* glwin, int xpos, int ypos) {
        static int last_xpos = xpos, last_ypos = ypos;
        int delta_xpos = xpos - last_xpos, delta_ypos = ypos - last_ypos;
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glwin));
        window->emit(VectorEventName::WINDOW_POSITION, VectorEvent { (double)xpos, (double)ypos, (double)delta_xpos, (double)delta_ypos });
        last_xpos = xpos;
        last_ypos = ypos;
    });
    glfwSetCursorPosCallback(_glfw_window, [](GLFWwindow* glwin, double xpos, double ypos) {
        static double last_xpos = xpos, last_ypos = ypos;
        double delta_xpos = xpos - last_xpos, delta_ypos = ypos - last_ypos;
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glwin));
        window->emit(VectorEventName::CURSOR_POSITION, VectorEvent { xpos, ypos, delta_xpos, delta_ypos });
        last_xpos = xpos;
        last_ypos = ypos;
    });
    glfwSetScrollCallback(_glfw_window, [](GLFWwindow* glwin, double xoffset, double yoffset) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glwin));
        window->emit(VectorEventName::SCROLL, VectorEvent { 0, 0, xoffset, yoffset });
    });

    glfwSetKeyCallback(_glfw_window, [](GLFWwindow* glwin, int key, int scancode, int action, int mods) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glwin));
        window->emit(InputEventName::KEY, InputEvent { key, scancode, action, mods });
    });
    glfwSetCharCallback(_glfw_window, [](GLFWwindow* glwin, unsigned int codepoint) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glwin));
        window->emit(InputEventName::CHARACTER, InputEvent { (int)codepoint, 0, 0, 0 });
    });
    glfwSetMouseButtonCallback(_glfw_window, [](GLFWwindow* glwin, int button, int action, int mods) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glwin));
        window->emit(InputEventName::MOUSE_BUTTON, InputEvent { button, 0, action, mods });
    });
}

} // namespace TotoGL
