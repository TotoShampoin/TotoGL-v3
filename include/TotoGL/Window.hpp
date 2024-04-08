#pragma once

#include <TotoGL/GL.hpp>

#include "Misc/EventListener.hpp"
#include <functional>
#include <string>

namespace TotoGL {

enum class FlagEventName {
    WINDOW_CLOSE,
    WINDOW_ICONIFY,
    WINDOW_MAXIMIZE,
    WINDOW_FOCUS,
    WINDOW_REFRESH,
};
struct FlagEvent {
    bool flag = false;
};

enum class VectorEventName {
    WINDOW_SIZE,
    FRAMEBUFFER_SIZE,
    CONTENT_SCALE_SIZE,
    WINDOW_POSITION,
    CURSOR_POSITION,
    SCROLL,
};
struct VectorEvent {
    double x = 0, y = 0;
    double dx = 0, dy = 0;
};

enum class InputEventName {
    KEY,
    CHARACTER,
    MOUSE_BUTTON,
};
struct InputEvent {
    int button = 0, scancode = 0, action = 0, mods = 0;
};

class Window {
public:
    Window(const int& width, const int& height, const std::basic_string<char>& title, const bool& fullscreen = false);
    Window(const Window&) = delete;
    ~Window();

    void draw(const std::function<void(void)>& callback);

    void makeContextCurrent();
    bool shouldClose();
    std::array<int, 2> size();

    GLFWwindow* glfwWindow() const { return _glfw_window; }

    void on(const FlagEventName& name, const std::function<void(FlagEvent)>& callback);
    void emit(const FlagEventName& name, const FlagEvent& value);
    void on(const VectorEventName& name, const std::function<void(VectorEvent)>& callback);
    void emit(const VectorEventName& name, const VectorEvent& value);
    void on(const InputEventName& name, const std::function<void(InputEvent)>& callback);
    void emit(const InputEventName& name, const InputEvent& value);

private:
    GLFWwindow* _glfw_window;
    EventListener<FlagEventName, FlagEvent> _flag_event_listener;
    EventListener<VectorEventName, VectorEvent> _vector_event_listener;
    EventListener<InputEventName, InputEvent> _input_event_listener;

    void bindEvents();

    static void init();
};

} // namespace TotoGL
