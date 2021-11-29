#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

namespace ORE
{
    class ImGuiLayer
    {
    public:
        ImGuiLayer(GLFWwindow *windowHandle);
        ~ImGuiLayer() = default;

        void OnAttach();
        void OnDetach();

        void Begin();
        void End();

        void saveImage(char *filepath);

        void SetDarkThemeColors();

    private:
        float m_Time = 0.0f;
        GLFWwindow *m_WindowHandle;
    };
} // namespace ORE
