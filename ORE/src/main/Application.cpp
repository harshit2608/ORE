#define DEBUG_OPENGL 1
#include "../ORE.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <stb_image.h>
#include <glm/gtc/type_ptr.hpp>

uint32_t SCREEN_WIDTH = 900;
uint32_t SCREEN_HEIGHT = 700;
std::string TITLE = "ORE";

float deltaTime = 0.0f;
float lastFrame = 0.0f;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
uint32_t loadCubemap(std::vector<std::string> faces);
void processInput(GLFWwindow *window);
void renderScene(const ORE::Shader &shader);
void renderCube();

bool blinn = false;
bool blinnKeyPressed = false;

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

    GLFWimage images[1];
    images[0].pixels = stbi_load("assets/logo/logo.png", &images[0].width, &images[0].height, 0, 4); // rgba channels
    if (!images[0].pixels)
    {
        ORE_CORE_ERROR("Fail to load logo {0}", images[0].pixels);
    }
    glfwSetWindowIcon(window, 1, images);
    stbi_image_free(images[0].pixels);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);
    glDisable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glEnable(GL_STENCIL_TEST);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    // glDisable(GL_DITHER);

    ORE::ImGuiLayer imguirender(window);
    imguirender.OnAttach();
    bool show_demo_window = true, show_another_window = false;

    ORE::Shader modelShader("assets/shaders/model.glsl");
    ORE::Shader skyboxShader("assets/shaders/skybox.glsl");
    // ORE::Shader blinphong("assets/shaders/blinphong.glsl");
    // ORE::Shader cubemapShader("assets/shaders/cubemap.glsl");

#pragma MODELS_SECTION
    // ORE::Model AMG("assets/models/AMG/AMG.obj", 1.0f);
    // scaleFactor = AMG.GetScaleFactor();
    // ORE::Model backpack("assets/models/backpack/backpack.obj", 1.0f);
    // scaleFactor = backpack.GetScaleFactor();
    // ORE::Model barrel("assets/models/barrel/Barrel_01.obj", 1.0f);
    // scaleFactor = barrel.GetScaleFactor();
    // ORE::Model city("assets/models/city/city.obj", 0.008f);
    // scaleFactor = city.GetScaleFactor();
    // ORE::Model helmet("assets/models/helmet/helmet.obj", 1.0f);
    // scaleFactor = helmet.GetScaleFactor();
    // ORE::Model sponza("assets/models/sponza/sponza.obj", 0.008f);
    // scaleFactor = sponza.GetScaleFactor();
    ORE::Model zorkiCamera("assets/models/zorkicamera/source/RC_zorki_Reduced/ZORKI_LENS.obj", 0.05f);
    scaleFactor = zorkiCamera.GetScaleFactor();

#pragma endregion

#pragma BLINN_PHONG
// float planeVertices[] = {
//     // positions            // normals         // texcoords
//     +10.0f, -0.5f, +10.0f, 0.0f, 1.0f, 0.0f, 10.0f, 0.0f,
//     -10.0f, -0.5f, +10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
//     -10.0f, -0.5f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 10.0f,

//     +10.0f, -0.5f, +10.0f, 0.0f, 1.0f, 0.0f, 10.0f, 0.0f,
//     -10.0f, -0.5f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 10.0f,
//     +10.0f, -0.5f, -10.0f, 0.0f, 1.0f, 0.0f, 10.0f, 10.0f};

// ORE::Ref<ORE::ManagerVertexArray> lightingvertexArray = ORE::ManagerVertexArray::Create();
// ORE::Ref<ORE::ManagerVertexBuffer> lightingvertexBuffer = ORE::ManagerVertexBuffer::Create(planeVertices, sizeof(planeVertices));
// ORE::BufferLayout lightinglayout = {
//     {ORE::ShaderDataType::Vec3, "a_Position"},
//     {ORE::ShaderDataType::Vec3, "a_Normal"},
//     {ORE::ShaderDataType::Vec2, "a_TexCoords"}};

// lightingvertexBuffer->SetLayout(lightinglayout);
// lightingvertexArray->AddVertexBuffer(lightingvertexBuffer);

// ORE::Ref<ORE::ManagerTexture2D> lightingshaderTexture = ORE::ManagerTexture2D::Create("assets/textures/wood.png");

// blinphong.Bind();
// blinphong.setInt("texture1", 0);
// blinphong.setBool("floorTexture", 0);
#pragma endregion

#pragma SKYBOX_SETUP
    float skyboxVertices[] = {
        // positions
        -1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        -1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f};

    ORE::Ref<ORE::ManagerVertexArray> SkyBoxvertexArray = ORE::ManagerVertexArray::Create();
    ORE::Ref<ORE::ManagerVertexBuffer> SkyBoxvertexBuffer = ORE::ManagerVertexBuffer::Create(skyboxVertices, sizeof(skyboxVertices));
    ORE::BufferLayout SkyBoxlayout = {
        {ORE::ShaderDataType::Vec3, "a_Position"},
        {ORE::ShaderDataType::Vec3, "a_Normal"},
        {ORE::ShaderDataType::Vec2, "a_TexCoords"}};

    SkyBoxvertexBuffer->SetLayout(SkyBoxlayout);
    SkyBoxvertexArray->AddVertexBuffer(SkyBoxvertexBuffer);

    uint32_t skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

    std::vector<std::string> faces{
        ("assets/textures/skybox/right.jpg"),
        ("assets/textures/skybox/left.jpg"),
        ("assets/textures/skybox/top.jpg"),
        ("assets/textures/skybox/bottom.jpg"),
        ("assets/textures/skybox/front.jpg"),
        ("assets/textures/skybox/back.jpg")};

    uint32_t cubemapTexture = loadCubemap(faces);

    modelShader.Bind();
    modelShader.setInt("skybox", 0);
    skyboxShader.Bind();
    skyboxShader.setInt("skybox", 0);

#pragma endregion

    ORE::Transformations entity = {glm::vec3(0.0f, 0.0f, 0.0f),
                                   glm::vec3(0.0f, 0.0f, 0.0f),
                                   glm::vec3(1.0f * scaleFactor, 1.0f * scaleFactor, 1.0f * scaleFactor)};

    glm::vec3 lightPos(0.0f, 0.0f, 0.0f);

    float lightIntensity = 0.05f;
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);
        glClearColor(0.1f, 0.1f, 0.1f, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        modelShader.Bind();
        // blinphong.Bind();

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model(1.0f);

        // render the loaded model
        // glm::mat4 model = glm::mat4(1.0f);
        // model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        // model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        // modelShader.setMat4("model", model);
        // backpack.Draw(shader);

#pragma MODEL_RENDER
        // Transform matrix for mesh
        model = glm::translate(model, entity.position);
        model = glm::rotate(model, glm::radians(entity.rotation.x), {1.0f, 0.0f, 0.0f});
        model = glm::rotate(model, glm::radians(entity.rotation.y), {0.0f, 1.0f, 0.0f});
        model = glm::rotate(model, glm::radians(entity.rotation.z), {0.0f, 0.0f, 1.0f});
        model = glm::scale(model, entity.scale);
        modelShader.setMat4("projection", projection);
        modelShader.setMat4("view", view);
        modelShader.setMat4("model", model);
        // AMG.Draw(modelShader);
        // backpack.Draw(modelShader);
        // barrel.Draw(modelShader);
        // city.Draw(modelShader);
        // helmet.Draw(modelShader);
        // sponza.Draw(modelShader);
        zorkiCamera.Draw(modelShader);
#pragma endregion

        // blinphong.setMat4("projection", projection);
        // blinphong.setMat4("view", view);
        // blinphong.setVec3("viewPos", camera.Position);
        // blinphong.setVec3("lightPos", lightPos);
        // blinphong.setInt("blinn", blinn);
        // blinphong.setFloat("intensity", lightIntensity);
        // lightingvertexArray->Bind();
        // lightingshaderTexture->Bind();
        // glDrawArrays(GL_TRIANGLES, 0, 6);

        // std::cout << (blinn ? "Blinn-Phong" : "Phong") << std::endl;

        // vertexArray->Bind();
        // for (uint32_t i = 0; i < 10; i++)
        // {
        //     glm::mat4 model = glm::mat4(1.0f);
        //     model = glm::translate(model, cubePositions[i]);
        //     float angle = 20.0f * i;
        //     model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        //    modelShader.setMat4("model", model);

        //     glDrawArrays(GL_TRIANGLES, 0, 36);
        // }

        // uint32_t count = vertexArray->GetIndexBuffer()->GetCount();
        // vertexArray->Bind();
        // glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);

#pragma CUBEMAP_RENDER
        glDepthFunc(GL_LEQUAL); // change depth function so depth test passes when values are equal to depth buffer's content
        skyboxShader.Bind();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
        skyboxShader.setMat4("view", view);
        skyboxShader.setMat4("projection", projection);
        // skybox cube
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default
#pragma endregion

        imguirender.Begin();
        ImGui::Begin("Main Controls");
        // ImGui::Text("Point light");
        ImGui::Text("Model transform");
        // ImGui::SliderFloat3("Model Translation", &entity.position.x, -10.0f, 10.0f);
        // ImGui::SliderFloat3("Model Rotation", &entity.rotation.x, -360.0f, 360.0f);
        // ImGui::SliderFloat3("Model Scale", &entity.scale.x, 0.0f, 15.0f);
        if (ImGui::TreeNode("Transform"))
        {
            ImGui::DragFloat3("Position", glm::value_ptr(entity.position), 0.1f);
            ImGui::DragFloat3("Rotation", glm::value_ptr(entity.rotation), 0.1f);
            ImGui::DragFloat3("Scale", glm::value_ptr(entity.scale), 0.1f);
            if (ImGui::Button("Reset model transform"))
            {
                entity.position = entity.rotation = glm::vec3(0.0f);
                entity.scale = glm::vec3(1.0f * scaleFactor);
            }
            ImGui::TreePop();
        }

        ImGui::Text("Light Controls");
        ImGui::SliderFloat("Intensity", &lightIntensity, 0.05f, 10.0f);
        ImGui::Text("Camera Controls");
        // ImGui::ShowDemoWindow(&show_demo_window);
        ImGui::End();
        imguirender.End();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    imguirender.OnDetach();

#pragma RESOURCES_FREE
    // vertexArray->UnBind();
    // vertexBuffer->UnBind();
    modelShader.UnBind();
    // lightingvertexArray->UnBind();
    // lightingvertexBuffer->UnBind();
    // blinphong.UnBind();
#pragma endregion
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
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && !blinnKeyPressed)
    {
        blinn = !blinn;
        blinnKeyPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE)
    {
        blinnKeyPressed = false;
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
    if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
    {
        camera.ProcessMouseMovement(xoffset, yoffset);
    }
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

void renderScene(const ORE::Shader &shader)
{
    // floor
    glm::mat4 model = glm::mat4(1.0f);
    shader.setMat4("model", model);
    // glBindVertexArray(planeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    // cubes
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setMat4("model", model);
    renderCube();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setMat4("model", model);
    renderCube();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0));
    model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    model = glm::scale(model, glm::vec3(0.25));
    shader.setMat4("model", model);
    renderCube();
}

uint32_t loadCubemap(std::vector<std::string> faces)
{
    uint32_t textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    stbi_set_flip_vertically_on_load(false);
    int width, height, nrChannels;
    for (uint32_t i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            ORE_CORE_ERROR("Cubemap texture failed to load at path: {0}", faces[i]);
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

uint32_t cubeVAO = 0;
uint32_t cubeVBO = 0;

void renderCube()
{
    // initialize (if necessary)
    // if (cubeVAO == 0)
    // {
    //     float vertices[] = {
    //         // back face
    //         -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
    //         1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,   // top-right
    //         1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,  // bottom-right
    //         1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,   // top-right
    //         -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
    //         -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,  // top-left
    //         // front face
    //         -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
    //         1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // bottom-right
    //         1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,   // top-right
    //         1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,   // top-right
    //         -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // top-left
    //         -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
    //         // left face
    //         -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // top-right
    //         -1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // top-left
    //         -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
    //         -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
    //         -1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom-right
    //         -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // top-right
    //                                                             // right face
    //         1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,     // top-left
    //         1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,   // bottom-right
    //         1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,    // top-right
    //         1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,   // bottom-right
    //         1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,     // top-left
    //         1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,    // bottom-left
    //         // bottom face
    //         -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
    //         1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,  // top-left
    //         1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,   // bottom-left
    //         1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,   // bottom-left
    //         -1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,  // bottom-right
    //         -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
    //         // top face
    //         -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
    //         1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // bottom-right
    //         1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,  // top-right
    //         1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // bottom-right
    //         -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
    //         -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f   // bottom-left
    //     };
    //     glGenVertexArrays(1, &cubeVAO);
    //     glGenBuffers(1, &cubeVBO);
    //     // fill buffer
    //     glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    //     glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //     // link vertex attributes
    //     glBindVertexArray(cubeVAO);
    //     glEnableVertexAttribArray(0);
    //     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    //     glEnableVertexAttribArray(1);
    //     glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    //     glEnableVertexAttribArray(2);
    //     glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    //     glBindBuffer(GL_ARRAY_BUFFER, 0);
    //     glBindVertexArray(0);
    // }
    // // render Cube
    // glBindVertexArray(cubeVAO);
    // glDrawArrays(GL_TRIANGLES, 0, 36);
    // glBindVertexArray(0);
}