cmake_minimum_required(VERSION 3.25)
project(music-visualizer)

set(CMAKE_CXX_STANDARD 17)
set(OpenGlLinkers -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl)
set(SDL_Linkers -lSDL2 -lSDL2_mixer)
set(LibSndFileLinkers -lsndfile)
set(fftwLinkers -lfftw3 -lm)

link_directories("/usr/local/src/glfw-3.3.9/build/src")
include_directories("/usr/local/src/glfw-3.3.9/include")
link_directories("${PROJECT_SOURCE_DIR}/external/glad/src")
include_directories("${PROJECT_SOURCE_DIR}/external/glad/include/")
include_directories("${PROJECT_SOURCE_DIR}/imgui")

add_executable(music-visualizer main.cpp
        ${PROJECT_SOURCE_DIR}/external/glad/src/glad.c
        IndexBuffer.h
        IndexBuffer.cpp
        Renderer.cpp
        Renderer.h
        Window.cpp
        Window.h
        Music.cpp
        Music.h
        Shader.cpp
        Shader.h
        VertexArray.h
        VertexArray.cpp
        VertexBuffer.cpp
        VertexLayout.cpp
        VertexLayout.h
        SoundData.cpp
        SoundData.h
        Error.cpp
        Error.h
        FloatDataView.cpp
        FloatDataView.h
        RenderCommand.cpp
        RenderCommand.h
        RendererAPI.cpp
        RendererAPI.h
        OpenGLRendererAPI.cpp
        OpenGLRendererAPI.h
        ${PROJECT_SOURCE_DIR}/imgui/imgui.cpp
        ${PROJECT_SOURCE_DIR}/imgui/imgui.h
        ${PROJECT_SOURCE_DIR}/imgui/imgui_demo.cpp
        ${PROJECT_SOURCE_DIR}/imgui/imgui_draw.cpp
        ${PROJECT_SOURCE_DIR}/imgui/imgui_internal.h
        ${PROJECT_SOURCE_DIR}/imgui/imgui_tables.cpp
        ${PROJECT_SOURCE_DIR}/imgui/imgui_widgets.cpp
        ${PROJECT_SOURCE_DIR}/imgui/backends/imgui_impl_glfw.cpp
        ${PROJECT_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3.cpp
        GUI.cpp
        GUI.h
        Spectrum.cpp
        Spectrum.h
        PerlinNoise.cpp
        PerlinNoise.h
        config.h
)

target_link_libraries(music-visualizer ${OpenGlLinkers} ${SDL_Linkers} ${LibSndFileLinkers} ${fftwLinkers})