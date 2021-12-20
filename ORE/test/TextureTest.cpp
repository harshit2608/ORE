#include "../src/ORE.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <stb_image.h>

const uint32_t SCREEN_WIDTH = 900;
uint32_t SCREEN_HEIGHT = 700;
std::string TITLE = "TextureTest";

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

int main()
{
    ORE::Log::Init();
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, TITLE.c_str(), nullptr, nullptr);

    if (window == nullptr)
    {
        ORE_CORE_ERROR("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    ORE::Context context(window);
    context.Init();

    ORE::Shader shader("assets\\shaders\\testShaders\\texturetest.glsl");
    ORE::Texture texture("assets\\textures\\testTextures\\container.jpg");

    float vertices[] = {
        // positions          // colors           // texture coords
        0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // top right
        0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f   // top left
    };

    uint32_t indices[] = {
        0, 1, 3, // first Triangle
        1, 2, 3  // second Triangle
    };

    ORE::Ref<ORE::ManagerVertexArray> vertexArray = ORE::ManagerVertexArray::Create();
    ORE::Ref<ORE::ManagerVertexBuffer> vertexBuffer = ORE::ManagerVertexBuffer::Create(vertices, sizeof(vertices));
    ORE::BufferLayout layout = {
        {ORE::ShaderDataType::Vec3, "a_Position"},
        {ORE::ShaderDataType::Vec3, "a_Color"},
        {ORE::ShaderDataType::Vec2, "a_TexCoords"}};

    vertexBuffer->SetLayout(layout);
    vertexArray->AddVertexBuffer(vertexBuffer);

    ORE::Ref<ORE::ManagerIndexBuffer> indexBuffer = ORE::ManagerIndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
    vertexArray->SetIndexBuffer(indexBuffer);

    ORE::ImGuiLayer imguirender(window);
    imguirender.OnAttach();

    glm::vec4 m_BackgroundColor = {0.1f, 0.1f, 0.1f, 1.0f};

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(m_BackgroundColor.r, m_BackgroundColor.g, m_BackgroundColor.b, m_BackgroundColor.a);
        glClear(GL_COLOR_BUFFER_BIT);
        imguirender.Begin();
        texture.Bind();
        shader.Bind();
        uint32_t count = vertexArray->GetIndexBuffer()->GetCount();
        vertexArray->Bind();
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);

        ImGui::ColorEdit4("BackGround Color", glm::value_ptr(m_BackgroundColor));
        ImGui::End();
        imguirender.End();
        // ImGui::Render();
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    imguirender.OnDetach();
    vertexArray->UnBind();
    vertexBuffer->UnBind();
    shader.UnBind();
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}
