#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

namespace ORE
{
    // void ImGuiinit(GLFWwindow *window)
    // {
    //     IMGUI_CHECKVERSION();
    //     ImGui::CreateContext();
    //     ImGuiIO &io = ImGui::GetIO();
    //     (void)io;

    //     io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    //     // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    //     io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;   // Enable Docking
    //     io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
    //     // io.ConfigViewportsNoAutoMerge = true;
    //     // io.ConfigViewportsNoTaskBarIcon = true;
    //     ImGui::StyleColorsDark();

    //     ImGuiStyle &style = ImGui::GetStyle();
    //     if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    //     {
    //         // style.WindowRounding = 0.0f;
    //         style.Colors[ImGuiCol_WindowBg].w = 0.f;
    //         style.Colors[ImGuiCol_DockingEmptyBg].w = 1.0f;
    //     }
    //     ImGui_ImplGlfw_InitForOpenGL(window, true);
    //     ImGui_ImplOpenGL3_Init("#version 450");
    //     auto &colors = ImGui::GetStyle().Colors;
    // }
    class ImGuiLayer
    {
    public:
        ImGuiLayer(GLFWwindow *windowHandle);
        ~ImGuiLayer() = default;

        void OnAttach();
        void OnDetach();

        void Begin();
        void End();

        void SetDarkThemeColors();

    private:
        float m_Time = 0.0f;
        GLFWwindow *m_WindowHandle;
    };
} // namespace ORE
