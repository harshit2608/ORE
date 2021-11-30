
#include "ImGuiLayer.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <stb_image_write.h>

// TEMPORARY
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace ORE
{
    ImGuiLayer::ImGuiLayer(GLFWwindow *windowHandle)
        : m_WindowHandle(windowHandle)
    {
    }

    void ImGuiLayer::OnAttach()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;   // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
        // io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
        // io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

        // Setup Dear ImGui style
        // ImGui::StyleColorsDark();
        // ImGui::StyleColorsClassic();

        ImGuiStyle &style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        SetDarkThemeColors();
        // ImGui::StyleColorsDark();
        // Setup Platform/Renderer bindings
        ImGui_ImplGlfw_InitForOpenGL(m_WindowHandle, true);
        ImGui_ImplOpenGL3_Init("#version 410");
    }

    void ImGuiLayer::OnDetach()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiLayer::Begin()
    {

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiLayer::End()
    {
        ImGuiIO &io = ImGui::GetIO();

        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow *backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }

    void ImGuiLayer::SetDarkThemeColors()
    {
        auto &style = ImGui::GetStyle();
        auto &colors = ImGui::GetStyle().Colors;

        style.TabRounding = 3.5f;
        style.FrameRounding = 3.5f;
        style.PopupRounding = 3.5f;
        style.ScrollbarRounding = 3.5f;
        style.GrabRounding = 3.5f;
        style.WindowTitleAlign = ImVec2(0.5f, 0.5f);

        ImVec4 blackMain, purple, blackLightShade, blackLightShade2;
        blackMain = {0.121f, 0.121f, 0.14f, 1.0f};
        blackLightShade = {0.141f, 0.145f, 0.192f, 1.0f};
        purple = {0.411f, 0.329f, 0.796f, 1.0f};
        blackLightShade2 = {0.172f, 0.172f, 0.227f, 1.0f};

        colors[ImGuiCol_WindowBg] = blackMain;

        // Headers
        colors[ImGuiCol_Header] = blackMain;
        colors[ImGuiCol_HeaderHovered] = purple;
        colors[ImGuiCol_HeaderActive] = blackLightShade;

        // Checkbox
        colors[ImGuiCol_CheckMark] = purple;

        // Buttons
        colors[ImGuiCol_Button] = blackMain;
        colors[ImGuiCol_ButtonHovered] = purple;
        colors[ImGuiCol_ButtonActive] = blackLightShade;

        // Frame BG
        colors[ImGuiCol_FrameBg] = blackMain;
        colors[ImGuiCol_FrameBgHovered] = purple;
        colors[ImGuiCol_FrameBgActive] = blackLightShade;

        // Tabs
        colors[ImGuiCol_Tab] = blackMain;
        colors[ImGuiCol_TabHovered] = blackLightShade2;
        colors[ImGuiCol_TabActive] = purple;
        colors[ImGuiCol_TabUnfocused] = blackLightShade;
        colors[ImGuiCol_TabUnfocusedActive] = blackMain;

        // Title
        colors[ImGuiCol_TitleBg] = blackMain;
        colors[ImGuiCol_TitleBgActive] = blackMain;
        colors[ImGuiCol_TitleBgCollapsed] = blackMain;
    }

    void ImGuiLayer::saveImage(const char *filepath)
    {
        int width, height;
        glfwGetFramebufferSize(m_WindowHandle, &width, &height);
        GLsizei nrChannels = 3;
        GLsizei stride = nrChannels * width;
        stride += (stride % 4) ? (4 - stride % 4) : 0;
        GLsizei bufferSize = stride * height;
        std::vector<char> buffer(bufferSize);
        glPixelStorei(GL_PACK_ALIGNMENT, 4);
        glReadBuffer(GL_FRONT);
        glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, buffer.data());
        stbi_flip_vertically_on_write(true);
        stbi_write_png(filepath, width, height, nrChannels, buffer.data(), stride);
    }
} // namespace ORE
