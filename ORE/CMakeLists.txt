project(Engine)

file(GLOB_RECURSE SRC CONFIGURE_DEPENDS "src/*.h" "src/*.cpp")
list(REMOVE_ITEM SRC "src/main/Application.cpp")

add_executable(${PROJECT_NAME} "src/main/Application.cpp" "${SRC}")

target_precompile_headers(${PROJECT_NAME} PRIVATE "src/pch.h" "src/pch.cpp")