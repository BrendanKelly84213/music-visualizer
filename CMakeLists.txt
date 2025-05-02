cmake_minimum_required(VERSION 3.16)
project(music-visualizer)

set(CMAKE_CXX_STANDARD 23)
set(OpenGlLinkers -lGL -lX11 -lpthread -lXrandr -lXi -ldl)
set(SDL_Linkers -lSDL2 -lSDL2_mixer)
set(LibSndFileLinkers -lsndfile)
set(fftwLinkers -lfftw3 -lm)

find_package(glfw3 3.3 REQUIRED)
find_package(glm REQUIRED)
find_package(SDL2 REQUIRED)
# https://answers.ros.org/question/253311/how-to-add-sdl2-library-in-cmake/
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2mixer REQUIRED SDL2_mixer)
# https://stackoverflow.com/questions/44277114/how-to-use-fftw-library-in-cmake
pkg_search_module(FFTW REQUIRED fftw3 IMPORTED_TARGET)

link_directories("${PROJECT_SOURCE_DIR}/external/glad/src")
include_directories("${PROJECT_SOURCE_DIR}/external/glad/include/")
include_directories("external/imgui")
include_directories("external/imnodes")

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
        external/imgui/imgui.cpp
        external/imgui/imgui.h
        external/imgui/imgui_demo.cpp
        external/imgui/imgui_draw.cpp
        external/imgui/imgui_internal.h
        external/imgui/imgui_tables.cpp
        external/imgui/imgui_widgets.cpp
        external/imgui/backends/imgui_impl_glfw.cpp
        external/imgui/backends/imgui_impl_opengl3.cpp
        external/imnodes/imnodes.cpp
        external/imnodes/imnodes.h
        external/imnodes/imnodes_internal.h
        GUI.cpp
        GUI.h
        Spectrum.cpp
        Spectrum.h
        PerlinNoise.cpp
        PerlinNoise.h
        config.h
        Application.cpp
        Application.h
        Graph.cpp
        Graph.h
        NodeEditor.cpp
        NodeEditor.h
        FrameBuffer.cpp
        FrameBuffer.h
)

target_link_libraries(music-visualizer glfw ${OpenGlLinkers} ${SDL_Linkers} ${LibSndFileLinkers} ${fftwLinkers})