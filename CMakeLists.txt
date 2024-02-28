cmake_minimum_required(VERSION 3.25)
project(trying-opengl)

set(CMAKE_CXX_STANDARD 17)
set(OpenGlLinkers -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl)

find_package(OpenGL REQUIRED)

link_directories("./include")
link_directories("/usr/local/src/glfw-3.3.9/build/src")
include_directories("/usr/local/src/glfw-3.3.9/include")
link_directories("/usr/local/src/glad/src")
include_directories("/usr/local/src/glad/include")

add_executable(music_visualizer main.cpp
        IndexBuffer.cpp
        Renderer.cpp
        Shader.h
        Texture.cpp
        VertexArray.cpp
        VertexBuffer.cpp)
