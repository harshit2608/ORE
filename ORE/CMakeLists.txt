project(Engine)

find_package(OpenGL REQUIRED)

file(GLOB_RECURSE SRC CONFIGURE_DEPENDS "src/*.h" "src/*.cpp")
list(REMOVE_ITEM SRC "src/main/Application.cpp")

option(DEBUG "Output to Debug dir" OFF)

add_executable(${PROJECT_NAME} "src/main/Application.cpp" "${SRC}")
add_executable(Scene2D_Test "test/2Dscene.cpp" "${SRC}")
add_executable(Texture_Test "test/TextureTest.cpp" "${SRC}")

target_include_directories(${PROJECT_NAME}
    PUBLIC
        external
        ${OPENGL_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        ${OPENGL_gl_LIBRARY}
        glfw
        Glad
        spdlog
        ImGui
        stb_image
        glm
        assimp
        imm32.lib
        ImGuiFileDialog
)

target_precompile_headers(${PROJECT_NAME} PRIVATE "src/pch.h" "src/pch.cpp")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRC})


message("==== Copying Assets folder and Imgui config into build directories ====")
if(DEBUG)
    file(COPY assets imgui.ini DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    file(COPY assets imgui.ini DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug)
else()
    file(COPY assets imgui.ini DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release)
endif()
message("==== Copying Completed!! ====")


target_include_directories(Scene2D_Test
    PUBLIC
        external
        ${OPENGL_INCLUDE_DIR}
)

target_link_libraries(Scene2D_Test
    PUBLIC
        ${OPENGL_gl_LIBRARY}
        glfw
        Glad
        spdlog
        ImGui
        stb_image
        glm
        assimp
        imm32.lib
        ImGuiFileDialog
)
target_precompile_headers(Scene2D_Test PRIVATE "src/pch.h" "src/pch.cpp")

# TEXTURE TEST================================================================================
target_include_directories(Texture_Test
    PUBLIC
        external
        ${OPENGL_INCLUDE_DIR}
)

target_link_libraries(Texture_Test
    PUBLIC
        ${OPENGL_gl_LIBRARY}
        glfw
        Glad
        spdlog
        ImGui
        stb_image
        glm
        assimp
        imm32.lib
        ImGuiFileDialog
)
target_precompile_headers(Texture_Test PRIVATE "src/pch.h" "src/pch.cpp")
