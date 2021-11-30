#define DEBUG_OPENGL 1
#include "../ORE.h"
#include "ApplicationUtils.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <stb_image.h>
#include <stb_image_write.h>
#include <glm/gtc/type_ptr.hpp>

uint32_t SCREEN_WIDTH = 900;
uint32_t SCREEN_HEIGHT = 700;
float ASPECT_RATIO = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;
std::string TITLE = "ORE";

float deltaTime = 0.0f;
float lastFrame = 0.0f;
int frameCount = 0;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
uint32_t loadCubemap(std::vector<std::string> faces);

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
    glfwWindowHint(GLFW_SAMPLES, 4);

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
        ORE_CORE_ERROR("Failed to load logo {0}", images[0].pixels);
    }
    glfwSetWindowIcon(window, 1, images);
    stbi_image_free(images[0].pixels);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_UNAVAILABLE);

    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);
    // glEnable(GL_MULTISAMPLE);
    // glCullFace(GL_BACK);
    glDisable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glEnable(GL_STENCIL_TEST);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    // glDisable(GL_DITHER);

    ORE::Shader modelShader("assets/shaders/model.glsl");
    ORE::Shader skyboxShader("assets/shaders/skybox.glsl");
    // ORE::Shader blinphong("assets/shaders/blinphong.glsl");
    // ORE::Shader cubemap("assets/shaders/cubemap.glsl");
    ORE::Shader lightCubeShader("assets/shaders/lightcube.glsl");

    ORE::Ref<ORE::ManagerVertexArray> m_SkyBoxvertexArray, m_CubevertexArray, m_LightCubevertexArray;
    ORE::Ref<ORE::ManagerVertexBuffer> m_SkyBoxvertexBuffer, m_CubevertexBuffer, m_LightCubevertexBuffer;

#pragma MODELS_SECTION
    // ORE::Model backpack("assets/models/backpack/backpack.obj", 1.0f);
    // scaleFactor = backpack.GetScaleFactor();
    // ORE::Model barrel("assets/models/barrel/Barrel_01.obj", 1.0f);
    // scaleFactor = barrel.GetScaleFactor();
    // ORE::Model bedroom("assets/models/bedroom/iscv2.obj", 0.05f);
    // scaleFactor = bedroom.GetScaleFactor();
    // ORE::Model city("assets/models/city/city.obj", 0.008f);
    // scaleFactor = city.GetScaleFactor();
    // ORE::Model gun("assets/models/gun/Cerberus_LP.FBX", 0.08f);
    // scaleFactor = gun.GetScaleFactor();
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
    ORE::skyBoxAttach(m_SkyBoxvertexArray, m_SkyBoxvertexBuffer);
    // ORE::cubeAttach(m_CubevertexArray, m_CubevertexBuffer);
    ORE::lightCubeAttach(m_LightCubevertexArray, m_LightCubevertexBuffer);

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

    ORE::Transformations object = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(1.0f * scaleFactor, 1.0f * scaleFactor, 1.0f * scaleFactor)};

    ORE::Transformations lights = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 1.0f)};

    glm::vec3 lightPos(0.0f, 3.0f, 0.0f);
    glm::vec4 m_BackgroundColor = {0.1f, 0.1f, 0.1f, 1.0f}, m_LightColor = {1.0f, 1.0f, 1.0f, 1.0f};
    float lightIntensity = 0.05f;
    bool load_skybox = 0, light_bloom = 0, polygon_mode = 0, anti_aliasing = 0, cull_face = 0;
    static int image_count = 0;
    std::string res = "Image.png";
    const char *image_outDir = res.c_str();

    ORE::ImGuiLayer imguirender(window);
    imguirender.OnAttach();
    bool show_demo_window = true;

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        imguirender.Begin();
        ImGui::Begin("Performance Status");
        frameCount++;
        // if (deltaTime >= 1.0)
        // {
        ImGui::ShowMetricsWindow();
        // ImGui::Text("Frame Time %.3f ms/frame (%.1f FPS)", 1000.0f / (double)frameCount, (double)frameCount);
        // frameCount = 0;
        lastFrame = currentFrame;
        // }
        ImGui::End();

        processInput(window);
        glClearColor(m_BackgroundColor.r, m_BackgroundColor.g, m_BackgroundColor.b, m_BackgroundColor.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        modelShader.Bind();

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), ASPECT_RATIO, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model(1.0f);

#pragma MODEL_RENDER
        // Transform matrix for mesh
        model = glm::translate(model, object.position);
        model = glm::rotate(model, glm::radians(object.rotation.x), {1.0f, 0.0f, 0.0f});
        model = glm::rotate(model, glm::radians(object.rotation.y), {0.0f, 1.0f, 0.0f});
        model = glm::rotate(model, glm::radians(object.rotation.z), {0.0f, 0.0f, 1.0f});
        model = glm::scale(model, object.scale);
        modelShader.setMat4("projection", projection);
        modelShader.setMat4("view", view);
        modelShader.setMat4("model", model);
        // backpack.Draw(modelShader);
        // barrel.Draw(modelShader);
        // bedroom.Draw(modelShader);
        // city.Draw(modelShader);
        // gun.Draw(modelShader);
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
        // m_CubevertexArray->Bind();
        // glDrawArrays(GL_TRIANGLES, 0, 36);
        // m_CubevertexArray->UnBind();

        lightCubeShader.Bind();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);
        glm::mat4 lightCubemodel(1.0f);

#pragma MODEL_RENDER
        // Transform matrix for mesh
        lightCubemodel = glm::translate(lightCubemodel, lights.position);
        lightCubemodel = glm::rotate(lightCubemodel, glm::radians(lights.rotation.x), {1.0f, 0.0f, 0.0f});
        lightCubemodel = glm::rotate(lightCubemodel, glm::radians(lights.rotation.y), {0.0f, 1.0f, 0.0f});
        lightCubemodel = glm::rotate(lightCubemodel, glm::radians(lights.rotation.z), {0.0f, 0.0f, 1.0f});
        lightCubemodel = glm::scale(lightCubemodel, lights.scale);
        lightCubeShader.setMat4("model", lightCubemodel);
        lightCubeShader.setVec4("lightColor", m_LightColor);

        m_LightCubevertexArray->Bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);
        m_LightCubevertexArray->UnBind();

#pragma CUBEMAP_RENDER
        glDepthFunc(GL_LEQUAL);
        skyboxShader.Bind();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
        skyboxShader.setMat4("view", view);
        skyboxShader.setMat4("projection", projection);

        m_SkyBoxvertexArray->Bind();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        if (load_skybox)
        {
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glBindVertexArray(0);
        m_SkyBoxvertexArray->UnBind();
        glDepthFunc(GL_LESS);
#pragma endregion

        ImGui::Begin("Menu");
        ImGui::Checkbox("Load Skybox", &load_skybox);
        ImGui::Checkbox("WireFrame Mode", &polygon_mode);
        ImGui::Checkbox("Anti Aliasing", &anti_aliasing);
        ImGui::Checkbox("CullFace", &cull_face);
        if (cull_face)
        {
            glEnable(GL_CULL_FACE);
        }
        else
        {
            glDisable(GL_CULL_FACE);
        }
        ImGui::ColorEdit4("BackGround Color", glm::value_ptr(m_BackgroundColor));
        if (polygon_mode)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        if (anti_aliasing)
        {
            glEnable(GL_MULTISAMPLE);
        }
        else
        {
            glDisable(GL_MULTISAMPLE);
        }
        if (ImGui::Button("Take ScreenShot"))
        {
            imguirender.saveImage(image_outDir);
            image_count++;
            res = "Image" + std::to_string(image_count);
            res += ".png";
            image_outDir = res.c_str();
        }
        ImGui::End();

        ImGui::Begin("ViewPort Controls");
        ImGui::Text("Object Controls");
        ImGui::DragFloat3("Position##objectPosition", glm::value_ptr(object.position), 0.1f);
        ImGui::DragFloat3("Rotation##objectRotation", glm::value_ptr(object.rotation), 0.1f);
        ImGui::DragFloat3("Scale##objectScale", glm::value_ptr(object.scale), 0.1f);
        if (ImGui::Button("Reset Model transform"))
        {
            object.position = object.rotation = glm::vec3(0.0f);
            object.scale = glm::vec3(1.0f * scaleFactor);
        }

        ImGui::Separator();
        ImGui::Text("Light Controls");
        ImGui::DragFloat3("Position##lightPosition", glm::value_ptr(lights.position), 0.1f);
        ImGui::DragFloat3("Rotation##lightRotation", glm::value_ptr(lights.rotation), 0.1f);
        ImGui::DragFloat3("Scale##lightScale", glm::value_ptr(lights.scale), 0.1f);
        ImGui::ColorEdit4("Light Color", glm::value_ptr(m_LightColor));
        // ImGui::ColorPicker3("Color##lightColor", light_color);
        if (ImGui::Button("Reset Light transform"))
        {
            lights.position = lights.rotation = glm::vec3(0.0f);
            lights.scale = glm::vec3(1.0f);
        }
        ImGui::DragFloat("Intensity", &lightIntensity, 0.05f);
        ImGui::Checkbox("Bloom", &light_bloom);

        ImGui::Separator();
        ImGui::Text("Camera Controls");
        ImGui::DragFloat("Zoom", &camera.Zoom, 0.1f);
        // ImGui::ShowDemoWindow(&show_demo_window);
        ImGui::End();

        imguirender.End();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    imguirender.OnDetach();
    modelShader.UnBind();
    lightCubeShader.UnBind();
    // blinphong.UnBind();
    ORE::skyBoxDetach(m_SkyBoxvertexArray, m_SkyBoxvertexBuffer);
    ORE::lightCubeDetach(m_LightCubevertexArray, m_LightCubevertexBuffer);
    // ORE::cubeDetach(m_CubevertexArray, m_CubevertexBuffer);
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
    ASPECT_RATIO = (float)width / height;
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