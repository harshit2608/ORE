#pragma once

struct GLFWwindow;

namespace ORE
{
    class Context
    {
    public:
        Context(GLFWwindow *windowHandle);

        void Init();
        void SwapBuffers();

    private:
        GLFWwindow *m_WindowHandle;
    };
} // namespace ORE
