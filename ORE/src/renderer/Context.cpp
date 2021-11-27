
#include "Context.h"
#include "../core/Log.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// #define DEBUG_OPENGL true

namespace ORE
{
    Context::Context(GLFWwindow *windowHandle)
        : m_WindowHandle(windowHandle)
    {
        ORE_CORE_ASSERT(windowHandle, "Window handle is null!")
    }

    void OpenGLMessageCallback(unsigned source,
                               unsigned type,
                               unsigned id,
                               unsigned severity,
                               int length,
                               const char *message,
                               const void *userParam)
    {
        {
            switch (severity)
            {
            case GL_DEBUG_SEVERITY_HIGH:
                ORE_CORE_CRITICAL(message);
                return;
            case GL_DEBUG_SEVERITY_MEDIUM:
                ORE_CORE_ERROR(message);
                return;
            case GL_DEBUG_SEVERITY_LOW:
                ORE_CORE_WARN(message);
                return;
            case GL_DEBUG_SEVERITY_NOTIFICATION:
                ORE_CORE_TRACE(message);
                return;
            }

            ORE_CORE_ASSERT(false, "Unknown severity level!");
        }
    }

    void Context::Init()
    {

#pragma region enable debug output
#if DEBUG_OPENGL
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(OpenGLMessageCallback, nullptr);

        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif
#pragma endregion

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
