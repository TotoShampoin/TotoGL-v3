#pragma once

#include <imgui.h>

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "Window.hpp"

namespace TotoGL {

inline void initImGui(Window& window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    ImGui_ImplGlfw_InitForOpenGL(window.glfwWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 460");

    auto& style = ImGui::GetStyle();
    style.FrameRounding = 2.;
    style.WindowRounding = 4.;
}

} // namespace TotoGL
