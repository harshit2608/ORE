# Glad=====================================================================================
set(GLAD_LIB_NAME "Glad")
message("=== BUILDING ${GLAD_LIB_NAME} ===")
set(GLAD_SRC_PATH "${GLAD_LIB_NAME}/src")
set(GLAD_INC_PATH "${GLAD_LIB_NAME}/include")

add_library( ${GLAD_LIB_NAME}
    STATIC
        "${GLAD_SRC_PATH}/glad.c"
)

target_include_directories(${GLAD_LIB_NAME}
    PUBLIC
        "${GLAD_INC_PATH}"
)

# GLFW=====================================================================================
set(GLFW_LIB_NAME "GLFW")
message("=== BUILDING ${GLFW_LIB_NAME} ===")
set(GLFW_INC_PATH ${GLFW_LIB_NAME}/include)

set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
#set(GLFW_VULKAN_SHARED  OFF CACHE BOOL "" FORCE)    # OFF by default
# set(BUILD_SHARED_LIBS   OFF CACHE BOOL "" FORCE)    # OFF by default
set(GLFW_INSTALL        OFF CACHE BOOL "" FORCE)

add_subdirectory(${GLFW_LIB_NAME})