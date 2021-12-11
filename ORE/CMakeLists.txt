project(Engine)

find_package(OpenGL REQUIRED)

file(GLOB_RECURSE SRC CONFIGURE_DEPENDS "src/*.h" "src/*.cpp")
list(REMOVE_ITEM SRC "src/main/Application.cpp")

add_executable(${PROJECT_NAME} "src/main/Application.cpp" "${SRC}")

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
file(COPY assets imgui.ini DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug)
file(COPY assets imgui.ini DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release)
file(COPY assets imgui.ini DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
