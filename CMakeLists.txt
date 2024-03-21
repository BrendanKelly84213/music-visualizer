cmake_minimum_required(VERSION 3.25)
project(music-visualizer)

set(CMAKE_CXX_STANDARD 17)
set(OpenGlLinkers -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl)
set(SDL_Linkers -lSDL2 -lSDL2_mixer)
set(LibSndFileLinkers -lsndfile)
set(fftwLinkers -lfftw3 -lm)

link_directories("/usr/local/src/glfw-3.3.9/build/src")
include_directories("/usr/local/src/glfw-3.3.9/include")
link_directories("/usr/local/src/glad/src")
include_directories( "/usr/local/src/glad/include")
include_directories("/home/brendan/dev/my-stuff/music-visualizer/imgui")

add_executable(music-visualizer main.cpp
        /usr/local/src/glad/src/glad.c
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
        /home/brendan/dev/my-stuff/music-visualizer/imgui/imgui.cpp
        /home/brendan/dev/my-stuff/music-visualizer/imgui/imgui.h
        /home/brendan/dev/my-stuff/music-visualizer/imgui/imgui_demo.cpp
        /home/brendan/dev/my-stuff/music-visualizer/imgui/imgui_draw.cpp
        /home/brendan/dev/my-stuff/music-visualizer/imgui/imgui_internal.h
        /home/brendan/dev/my-stuff/music-visualizer/imgui/imgui_tables.cpp
        /home/brendan/dev/my-stuff/music-visualizer/imgui/imgui_widgets.cpp
        /home/brendan/dev/my-stuff/music-visualizer/imgui/backends/imgui_impl_glfw.cpp
        /home/brendan/dev/my-stuff/music-visualizer/imgui/backends/imgui_impl_opengl3.cpp
        GUI.cpp
        GUI.h
        Spectrum.cpp
        Spectrum.h
)

target_link_libraries(music-visualizer ${OpenGlLinkers} ${SDL_Linkers} ${LibSndFileLinkers} ${fftwLinkers})