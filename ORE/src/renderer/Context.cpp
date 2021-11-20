
#include "Context.h"
#include "../core/Log.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace ORE
{
    Context::Context(GLFWwindow *windowHandle)
        : m_WindowHandle(windowHandle)
    {
        ORE_CORE_ASSERT(windowHandle, "Window handle is null!")
    }

    void Context::Init()
    {

        glfwMakeContextCurrent(m_WindowHandle);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        ORE_CORE_ASSERT(status, "Failed to initialize Glad!");

        int major, minor, nrAttributes;
        glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
        glGetIntegerv(GL_MAJOR_VERSION, &major);
        glGetIntegerv(GL_MINOR_VERSION, &minor);
        glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);

        ORE_CORE_INFO("OpenGL Info:");
        ORE_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
        ORE_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
        ORE_CORE_INFO("  Version (string): {0}", glGetString(GL_VERSION));
        ORE_CORE_INFO("  Version (integer): {0}.{1}", major, minor);
        ORE_CORE_INFO("  GLSL Version: {0}", glGetString(GL_SHADING_LANGUAGE_VERSION));
        ORE_CORE_INFO("  Maximum number of vertex attributes supported: {0}", nrAttributes);

        ORE_CORE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "ORE requires at least OpenGL version 4.5!");
    }

    void Context::SwapBuffers()
    {
        glfwSwapBuffers(m_WindowHandle);
    }
} // namespace ORE
