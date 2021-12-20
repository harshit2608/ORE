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

    ORE::Shader squareShader("assets\\shaders\\testShaders\\flatcolortest.glsl");

    // float vertices[3 * 7] = {
    //     -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
    //     0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
    //     0.0f, 0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f};

    // uint32_t indices[3] = {0, 1, 2};

    // ORE::Ref<ORE::ManagerVertexArray> vertexArray = ORE::ManagerVertexArray::Create();
    // ORE::Ref<ORE::ManagerVertexBuffer> vertexBuffer = ORE::ManagerVertexBuffer::Create(vertices, sizeof(vertices));
    // ORE::BufferLayout layout = {
    //     {ORE::ShaderDataType::Vec3, "a_Position"},
    //     {ORE::ShaderDataType::Vec3, "a_Color"}};

    // vertexBuffer->SetLayout(layout);
    // vertexArray->AddVertexBuffer(vertexBuffer);

    // ORE::Ref<ORE::ManagerIndexBuffer> indexBuffer = ORE::ManagerIndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
    // vertexArray->SetIndexBuffer(indexBuffer);

    // SQUARE=================
    float squareVertices[5 * 4] = {
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f};

    uint32_t squareIndices[6] = {0, 1, 2, 2, 3, 0};
    ORE::Ref<ORE::ManagerVertexArray> m_SquareVA = ORE::ManagerVertexArray::Create();
    ORE::Ref<ORE::ManagerVertexBuffer> squareVB = ORE::ManagerVertexBuffer::Create(squareVertices, sizeof(squareVertices));
    ORE::BufferLayout layout = {
        {ORE::ShaderDataType::Vec3, "a_Position"},
        {ORE::ShaderDataType::Vec2, "a_TexCoord"}};

    squareVB->SetLayout(layout);
    m_SquareVA->AddVertexBuffer(squareVB);

    ORE::Ref<ORE::ManagerIndexBuffer> squareIB = ORE::ManagerIndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
    m_SquareVA->SetIndexBuffer(squareIB);

    ORE::ImGuiLayer imguirender(window);
    imguirender.OnAttach();

    glm::vec4 m_BackgroundColor = {0.1f, 0.1f, 0.1f, 1.0f};
    glm::vec3 m_ShapeColor = {0.2f, 0.3f, 0.8f};
    squareShader.Bind();
    squareShader.setVec3("u_Color", m_ShapeColor);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(m_BackgroundColor.r, m_BackgroundColor.g, m_BackgroundColor.b, m_BackgroundColor.a);
        glClear(GL_COLOR_BUFFER_BIT);
        imguirender.Begin();
        squareShader.setVec3("u_Color", m_ShapeColor);
        // uint32_t count = vertexArray->GetIndexBuffer()->GetCount();
        // vertexArray->Bind();
        uint32_t count = m_SquareVA->GetIndexBuffer()->GetCount();
        m_SquareVA->Bind();
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);

        ImGui::ColorEdit4("BackGround Color", glm::value_ptr(m_BackgroundColor));
        ImGui::ColorEdit3("Shape Color", glm::value_ptr(m_ShapeColor));
        ImGui::End();
        imguirender.End();
        // ImGui::Render();
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    imguirender.OnDetach();
    // vertexArray->UnBind();
    // vertexBuffer->UnBind();
    m_SquareVA->UnBind();
    squareVB->UnBind();
    squareShader.UnBind();
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
