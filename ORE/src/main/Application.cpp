#define DEBUG_OPENGL 1
#include "../ORE.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <stb_image.h>

const uint32_t SCREEN_WIDTH = 900;
const uint32_t SCREEN_HEIGHT = 700;
std::string TITLE = "ORE";

float deltaTime = 0.0f;
float lastFrame = 0.0f;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

ORE::Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;
bool firstMouse = true;
float scaleFactor = 1.0f;

int main()
{
    ORE::Log::Init();

    glfwInit();

#if DEBUG_OPENGL
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif
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
    ORE::Context context(window);
    context.Init();
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    glDisable(GL_DITHER);

    stbi_set_flip_vertically_on_load(true);

    ORE::Shader shader("assets/shaders/model.glsl");

    ORE::Model backpack("assets/models/backpack/backpack.obj");
    // ORE::Model sponza("assets/models/sponza/sponza.obj");
    // ORE::Model helmet("assets/models/helmet/source/HelmetPresentationLightMap.fbx.fbx");
    // ORE::Model AMG("assets/models/AMG/AMG.obj");
    // ORE::Model zorkiCamera("assets/models/camera/source/RC_zorki_Reduced/ZORKI_LENS.obj");

    // ORE::Texture texture1("assets/textures/container.jpg");
    // ORE::Texture texture2("assets/textures/awesomeface.png");

    // float vertices[] = {
    //     // positions          // colors           // texture coords
    //     0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // top right
    //     0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // bottom right
    //     -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom left
    //     -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f   // top left
    // };

    // uint32_t indices[] = {
    //     0, 1, 3, // first Triangle
    //     1, 2, 3  // second Triangle
    // };

    // float vertices[] = {
    //     -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
    //     0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
    //     0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    //     0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    //     -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
    //     -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

    //     -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    //     0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    //     0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
    //     0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
    //     -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
    //     -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

    //     -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    //     -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    //     -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    //     -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    //     -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    //     -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

    //     0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    //     0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    //     0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    //     0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    //     0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    //     0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

    //     -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    //     0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
    //     0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    //     0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    //     -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    //     -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

    //     -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
    //     0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    //     0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    //     0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    //     -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
    //     -0.5f, 0.5f, -0.5f, 0.0f, 1.0f};

    // // world space positions of our cubes
    // glm::vec3 cubePositions[] = {
    //     glm::vec3(0.0f, 0.0f, 0.0f),
    //     glm::vec3(2.0f, 5.0f, -15.0f),
    //     glm::vec3(-1.5f, -2.2f, -2.5f),
    //     glm::vec3(-3.8f, -2.0f, -12.3f),
    //     glm::vec3(2.4f, -0.4f, -3.5f),
    //     glm::vec3(-1.7f, 3.0f, -7.5f),
    //     glm::vec3(1.3f, -2.0f, -2.5f),
    //     glm::vec3(1.5f, 2.0f, -2.5f),
    //     glm::vec3(1.5f, 0.2f, -1.5f),
    //     glm::vec3(-1.3f, 1.0f, -1.5f)};

    // ORE::Ref<ORE::ManagerVertexArray> vertexArray = ORE::ManagerVertexArray::Create();
    // ORE::Ref<ORE::ManagerVertexBuffer> vertexBuffer = ORE::ManagerVertexBuffer::Create(vertices, sizeof(vertices));
    // ORE::BufferLayout layout = {
    //     {ORE::ShaderDataType::Vec3, "a_Position"},
    //     {ORE::ShaderDataType::Vec3, "a_Color"},
    //     {ORE::ShaderDataType::Vec2, "a_TexCoords"}};

    // ORE::BufferLayout layout = {
    //     {ORE::ShaderDataType::Vec3, "a_Position"},
    //     {ORE::ShaderDataType::Vec2, "a_TexCoords"}};

    // vertexBuffer->SetLayout(layout);
    // vertexArray->AddVertexBuffer(vertexBuffer);

    // ORE::Ref<ORE::ManagerIndexBuffer> indexBuffer = ORE::ManagerIndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
    // vertexArray->SetIndexBuffer(indexBuffer);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 450");
    bool show_demo_window = true, show_another_window = false;

    ORE::Transformations entity = {glm::vec3(0.0f, 0.0f, 0.0f),
                                   glm::vec3(0.0f, 0.0f, 0.0f),
                                   glm::vec3(1.0f * scaleFactor, 1.0f * scaleFactor, 1.0f * scaleFactor)};

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);
        glClearColor(0.1f, 0.1f, 0.1f, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // texture1.Bind(0);
        // texture2.Bind(1);
        shader.Bind();

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        // render the loaded model
        // glm::mat4 model = glm::mat4(1.0f);
        // model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        // model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        // shader.setMat4("model", model);
        // backpack.Draw(shader);

        // Transform matrix for mesh
        glm::mat4 model(1.0f);
        model = glm::translate(model, entity.position);
        model = glm::rotate(model, glm::radians(entity.rotation.x), {1.0f, 0.0f, 0.0f});
        model = glm::rotate(model, glm::radians(entity.rotation.y), {0.0f, 1.0f, 0.0f});
        model = glm::rotate(model, glm::radians(entity.rotation.z), {0.0f, 0.0f, 1.0f});
        model = glm::scale(model, entity.scale);
        shader.setMat4("model", model);
        backpack.Draw(shader);
        // sponza.Draw(shader);
        // helmet.Draw(shader);
        // AMG.Draw(shader);
        // zorkiCamera.Draw(shader);

        // vertexArray->Bind();
        // for (unsigned int i = 0; i < 10; i++)
        // {
        //     glm::mat4 model = glm::mat4(1.0f);
        //     model = glm::translate(model, cubePositions[i]);
        //     float angle = 20.0f * i;
        //     model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        //     shader.setMat4("model", model);

        //     glDrawArrays(GL_TRIANGLES, 0, 36);
        // }

        // uint32_t count = vertexArray->GetIndexBuffer()->GetCount();
        // vertexArray->Bind();
        // glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Main Controls");
        // ImGui::Text("Point light");
        ImGui::Text("Model transform");
        ImGui::SliderFloat3("Model Translation", &entity.position.x, -10.0f, 10.0f);
        ImGui::SliderFloat3("Model Rotation", &entity.rotation.x, -360.0f, 360.0f);
        ImGui::SliderFloat3("Model Scale", &entity.scale.x, 0.0f, 15.0f);

        if (ImGui::Button("Reset model transform"))
        {
            entity.position = entity.rotation = glm::vec3(0.0f);
            entity.scale = glm::vec3(1.0f * scaleFactor);
        }

        ImGui::Text("Camera Controls");
        // ImGui::ShowDemoWindow(&show_demo_window);
        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        // ImGui::Render();
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // vertexArray->UnBind();
    // vertexBuffer->UnBind();
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

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(ORE::FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(ORE::BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(ORE::LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(ORE::RIGHT, deltaTime);
    }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        camera.ProcessMouseMovement(xoffset, yoffset);
    }
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}