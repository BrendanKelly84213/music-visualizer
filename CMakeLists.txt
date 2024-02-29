cmake_minimum_required(VERSION 3.25)
project(music-visualizer)

set(CMAKE_CXX_STANDARD 17)
set(OpenGlLinkers -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl)
set(SDL_Linkers -lSDL2 -lSDL2_mixer)

link_directories("/usr/local/src/glfw-3.3.9/build/src")
include_directories("/usr/local/src/glfw-3.3.9/include")
link_directories("/usr/local/src/glad/src")
include_directories( "/usr/local/src/glad/include")

add_executable(music-visualizer main.cpp
        /usr/local/src/glad/src/glad.c
        IndexBuffer.h
        IndexBuffer.cpp
        include/Window.cpp
        include/Window.h
)

target_link_libraries(music-visualizer ${OpenGlLinkers} ${SDL_Linkers})