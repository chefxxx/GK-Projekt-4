cmake_minimum_required(VERSION 3.26)
project(gk_final_0)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(dependencies)

add_subdirectory(dependencies/glfw-3.4
        ${CMAKE_BINARY_DIR}/glfw_build)
add_subdirectory(dependencies/glad)
add_subdirectory(dependencies/assimp)

set (PROJECT_SRC
        src/main.cpp
)

add_executable(gk_final_0 src/main.cpp
        src/primitives/myWindow.cpp
        src/primitives/myWindow.h
        src/primitives/FlyCamera.h
        src/primitives/Model.h
        src/primitives/Mesh.h
        dependencies/stb_image.cpp
        src/primitives/Shader.cpp
        src/primitives/Model.cpp
        src/primitives/FlyCamera.cpp
        src/primitives/LightSource.cpp
        src/primitives/LightSource.h)

target_link_libraries(gk_final_0 ${OPENGL_LIBRARIES} glfw glad assimp::assimp)
