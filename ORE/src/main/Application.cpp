#define DEBUG_OPENGL 1

#ifdef _MSVC_LANG
#define _CRT_SECURE_NO_WARNINGS
#endif
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
bool firstMouse = true;

ORE::Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;
float scaleFactor = 1.0f;

// static void HelpMarker(const char *desc)
// {
//     ImGui::TextDisabled("(?)");
//     if (ImGui::IsItemHovered())
//     {
//         ImGui::BeginTooltip();
//         ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
//         ImGui::TextUnformatted(desc);
//         ImGui::PopTextWrapPos();
//         ImGui::EndTooltip();
//     }
// }

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
    // glEnable(GL_b_cullFace);
    // glEnable(GL_MULTISAMPLE);
    // glCullFace(GL_BACK);
    glDisable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_STENCIL_TEST);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    glDisable(GL_DITHER);

    // ORE::Shader modelShader("assets/shaders/model.glsl");
    ORE::Shader skyboxShader("assets/shaders/skybox.glsl");
    // ORE::Shader modelShader("assets/shaders/blinphong.glsl");
    // ORE::Shader cubemap("assets/shaders/cubemap.glsl");
    ORE::Shader lightCubeShader("assets/shaders/lightcube.glsl");
    ORE::Shader lightingShader("assets/shaders/lighting.glsl");

    ORE::Ref<ORE::ManagerVertexArray> m_SkyBoxvertexArray, m_CubevertexArray, m_LightCubevertexArray;
    ORE::Ref<ORE::ManagerVertexBuffer> m_SkyBoxvertexBuffer, m_CubevertexBuffer, m_LightCubevertexBuffer;

#pragma MODELS_SECTION
    ORE::Timer tm("Model Loading");
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
    // ORE::Model hanuman("assets/models/hanuman/source/Hanuman/Hanuman.fbx", 0.05f);
    // scaleFactor = hanuman.GetScaleFactor();
    // ORE::Model helmet("assets/models/helmet/helmet.obj", 1.0f);
    // scaleFactor = helmet.GetScaleFactor();
    // ORE::Model sponza("assets/models/sponza/sponza.obj", 0.008f);
    // scaleFactor = sponza.GetScaleFactor();
    ORE::Model zorkiCamera("assets/models/zorkicamera/source/RC_zorki_Reduced/ZORKI_LENS.obj", 0.08f);
    scaleFactor = zorkiCamera.GetScaleFactor();

    // ORE::Model plazaNightTime("assets/models/plaza-night-time/source/plaza01/plaza01_night.FBX", 0.05f);
    // scaleFactor = plazaNightTime.GetScaleFactor();

    // ORE::Model mirrorEdgeAppartment("assets/models/mirrors_edge_apartment/scene.gltf", 0.008f);
    // scaleFactor = mirrorEdgeAppartment.GetScaleFactor();
    // ORE::Model scifi_corridor("assets/models/scifi_corridor/source/hallway/hallway.FBX", 0.05f);
    // scaleFactor = scifi_corridor.GetScaleFactor();
    // ORE::Model american_muscle_71("assets/models/american_muscle_71/source/Phoenix445.fbx", 0.05f);
    // scaleFactor = american_muscle_71.GetScaleFactor();
    // ORE::Model datsun1972("assets/models/1972_datsun_gt/source/datsun240k.fbx", 0.05f);
    // scaleFactor = datsun1972.GetScaleFactor();
    // ORE::Model porsche911_1975("assets/models/1975_porsche_911_930_turbo/scene.gltf", 0.05f);
    // scaleFactor = porsche911_1975.GetScaleFactor();
    tm.PrintTime();
    // vector<std::string> modelName;

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

    // blinphong.Bind();/
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

    // modelShader.Bind();
    // modelShader.setInt("skybox", 0);
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
        glm::vec3(0.5f, 0.5f, 0.5f)};

    // lightingShader.setFloat("light.constant", 1.0f);
    // lightingShader.setFloat("light.linear", 0.09f);
    // lightingShader.setFloat("light.quadratic", 0.032f);

    // material properties

    glm::vec3 lightPos(0.0f, 3.0f, 0.0f);
    glm::vec4 m_BackgroundColor = {0.1f, 0.1f, 0.1f, 1.0f}, m_LightColor = {1.0f, 1.0f, 1.0f, 1.0f};
    float m_lightIntensity = 0.5f, m_materialShininess = 32.0f, m_lightingDiffuse(0.8f), m_lightingAmbient(0.1f);
    bool b_load_skybox = 0, b_lightBloom = 0, b_polygonMode = 0, b_antiAliasing = 0, b_cullFace = 0, b_imguiUITheme = 0;
    ;
    static int image_count = 0;
    std::string res = "Image.png";
    const char *image_outDir = res.c_str();

    ORE::ImGuiLayer imguirender(window);
    imguirender.OnAttach();
    bool show_demo_window = true;

    lightingShader.Bind();
    lightingShader.setInt("material.diffuse", 0);
    lightingShader.setInt("material.specular", 1);
    // lightingShader.setFloat("bloomFactor", 1.0f);

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        imguirender.Begin();
        ORE::ScenePanel sceneRender;
        ImGui::Begin("Performance Status");
        imguirender.performanceLog();
        ImGui::End();

        processInput(window);
        glClearColor(m_BackgroundColor.r, m_BackgroundColor.g, m_BackgroundColor.b, m_BackgroundColor.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lightingShader.Bind();
        lightingShader.setVec3("light.position", lights.position);
        lightingShader.setVec3("light.direction", lights.scale);
        lightingShader.setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
        lightingShader.setFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));
        lightingShader.setVec3("viewPos", camera.Position);

        // light properties
        lightingShader.setVec3("light.ambient", glm::vec3(m_lightingAmbient));
        lightingShader.setVec3("light.diffuse", glm::vec3(m_lightingDiffuse));
        lightingShader.setVec3("light.specular", glm::vec3(1.0f));
        lightingShader.setVec4("objColor", m_LightColor);
        lightingShader.setFloat("light.constant", 1.0f);
        lightingShader.setFloat("light.linear", 0.09f);
        lightingShader.setFloat("light.quadratic", 0.032f);

        // material properties
        lightingShader.setFloat("material.shininess", m_materialShininess);

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
        // modelShader.setMat4("projection", projection);
        // modelShader.setMat4("view", view);
        // modelShader.setMat4("model", model);

        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);
        lightingShader.setMat4("model", model);

        // backpack.Draw(modelShader);
        // barrel.Draw(modelShader);
        // bedroom.Draw(modelShader);
        // city.Draw(modelShader);
        // gun.Draw(modelShader);
        // hanuman.Draw(modelShader);
        // helmet.Draw(modelShader);
        // sponza.Draw(modelShader);
        zorkiCamera.Draw(lightingShader);

        // plazaNightTime.Draw(modelShader);

        // mirrorEdgeAppartment.Draw(modelShader);
        // scifi_corridor.Draw(modelShader);
        // american_muscle_71.Draw(modelShader);
        // porsche911_1975.Draw(modelShader);
        // datsun1972.Draw(modelShader);
        // tm.PrintTime();
#pragma endregion

        // blinphong.setMat4("projection", projection);
        // blinphong.setMat4("view", view);
        // blinphong.setVec3("viewPos", camera.Position);
        // blinphong.setVec3("lightPos", lightPos);
        // blinphong.setInt("blinn", blinn);
        // blinphong.setFloat("intensity", m_lightIntensity);
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
        if (b_load_skybox)
        {
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glBindVertexArray(0);
        m_SkyBoxvertexArray->UnBind();
        glDepthFunc(GL_LESS);
#pragma endregion

        ImGui::Begin("Menu");
        ImGui::Checkbox("ImGui Purple UI", &b_imguiUITheme);
        ImGui::Checkbox("Load Skybox", &b_load_skybox);
        ImGui::Checkbox("WireFrame Mode", &b_polygonMode);
        ImGui::Checkbox("Anti Aliasing", &b_antiAliasing);
        ImGui::Checkbox("CullFace", &b_cullFace);
        ImGui::ColorEdit4("BackGround Color", glm::value_ptr(m_BackgroundColor));

        if (b_cullFace)
        {
            glEnable(GL_CULL_FACE);
        }
        else
        {
            glDisable(GL_CULL_FACE);
        }

        if (b_polygonMode)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        if (b_antiAliasing)
        {
            glEnable(GL_MULTISAMPLE);
        }
        else
        {
            glDisable(GL_MULTISAMPLE);
        }

        if (b_imguiUITheme)
        {
            imguirender.SetDarkThemeColors();
        }
        else
        {
            ImGui::StyleColorsDark();
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
        sceneRender.DrawVec3Control("Translation", (object.position), 1);
        sceneRender.DrawVec3Control("Rotation", (object.rotation), 1);
        sceneRender.DrawVec3Control("Scale", (object.scale), 1);
        if (ImGui::Button("Reset Model transform"))
        {
            object.position = object.rotation = glm::vec3(0.0f);
            object.scale = glm::vec3(1.0f * scaleFactor);
        }

        ImGui::Separator();
        ImGui::Text("Light Controls");
        sceneRender.DrawVec3Control("Translation", (lights.position), 2);
        sceneRender.DrawVec3Control("Rotation", (lights.rotation), 2);
        sceneRender.DrawVec3Control("Scale", (lights.scale), 2);
        ImGui::ColorEdit4("Light Color", glm::value_ptr(m_LightColor));
        ImGui::DragFloat("Diffuse", &m_lightingDiffuse, 0.05f);
        ImGui::DragFloat("Ambient", &m_lightingAmbient, 0.01f);
        // ImGui::DragFloat("Intensity", &m_lightIntensity, 0.05f);
        if (ImGui::Button("Reset Light transform"))
        {
            lights.position = lights.rotation = glm::vec3(0.0f);
            lights.scale = glm::vec3(0.5f);
            m_lightingDiffuse = (0.8f);
            m_lightingAmbient = (0.1f);
        }

        // ImGui::Checkbox("Bloom", &b_lightBloom);
        // if (b_lightBloom)
        // {
        //     lightingShader.setFloat("bloomFactor", m_lightIntensity);
        // }
        // else
        // {
        //     lightingShader.setFloat("bloomFactor", 1.0f);
        // }

        ImGui::Separator();
        ImGui::Text("Camera Controls");
        ImGui::DragFloat("Zoom", &camera.Zoom, 0.1f);
        ImGui::End();

        // if (ImGui::TreeNode("Combo"))
        // {
        //     // Expose flags as checkbox for the demo
        //     static ImGuiComboFlags flags = 0;
        //     ImGui::CheckboxFlags("ImGuiComboFlags_PopupAlignLeft", &flags, ImGuiComboFlags_PopupAlignLeft);
        //     ImGui::SameLine();
        //     HelpMarker("Only makes a difference if the popup is larger than the combo");
        //     if (ImGui::CheckboxFlags("ImGuiComboFlags_NoArrowButton", &flags, ImGuiComboFlags_NoArrowButton))
        //         flags &= ~ImGuiComboFlags_NoPreview; // Clear the other flag, as we cannot combine both
        //     if (ImGui::CheckboxFlags("ImGuiComboFlags_NoPreview", &flags, ImGuiComboFlags_NoPreview))
        //         flags &= ~ImGuiComboFlags_NoArrowButton; // Clear the other flag, as we cannot combine both

        //     // Using the generic BeginCombo() API, you have full control over how to display the combo contents.
        //     // (your selection data could be an index, a pointer to the object, an id for the object, a flag intrusively
        //     // stored in the object itself, etc.)
        //     const char *items[] = {"AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK", "LLLLLLL", "MMMM", "OOOOOOO"};
        //     static int item_current_idx = 0;                           // Here we store our selection data as an index.
        //     const char *combo_preview_value = items[item_current_idx]; // Pass in the preview value visible before opening the combo (it could be anything)
        //     if (ImGui::BeginCombo("combo 1", combo_preview_value, flags))
        //     {
        //         for (int n = 0; n < IM_ARRAYSIZE(items); n++)
        //         {
        //             const bool is_selected = (item_current_idx == n);
        //             if (ImGui::Selectable(items[n], is_selected))
        //                 item_current_idx = n;

        //             // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
        //             if (is_selected)
        //                 ImGui::SetItemDefaultFocus();
        //         }
        //         ImGui::EndCombo();
        //     }

        //     // Simplified one-liner Combo() API, using values packed in a single constant string
        //     // This is a convenience for when the selection set is small and known at compile-time.
        //     static int item_current_2 = 0;
        //     ImGui::Combo("combo 2 (one-liner)", &item_current_2, "aaaa\0bbbb\0cccc\0dddd\0eeee\0\0");

        //     // Simplified one-liner Combo() using an array of const char*
        //     // This is not very useful (may obsolete): prefer using BeginCombo()/EndCombo() for full control.
        //     static int item_current_3 = -1; // If the selection isn't within 0..count, Combo won't display a preview
        //     ImGui::Combo("combo 3 (array)", &item_current_3, items, IM_ARRAYSIZE(items));

        //     // Simplified one-liner Combo() using an accessor function
        //     struct Funcs
        //     {
        //         static bool ItemGetter(void *data, int n, const char **out_str)
        //         {
        //             *out_str = ((const char **)data)[n];
        //             return true;
        //         }
        //     };
        //     static int item_current_4 = 0;
        //     ImGui::Combo("combo 4 (function)", &item_current_4, &Funcs::ItemGetter, items, IM_ARRAYSIZE(items));

        //     ImGui::TreePop();
        // }
        imguirender.End();

        context.SwapBuffers();
        glfwPollEvents();
    }

    imguirender.OnDetach();
    // modelShader.UnBind();
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
    stbi_set_flip_vertically_on_load(true);

    return textureID;
}
