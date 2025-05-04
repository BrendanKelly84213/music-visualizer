cmake_minimum_required(VERSION 3.16)
project(rework)

set(CMAKE_CXX_STANDARD 23)
set(OpenGlLinkers -lGL -lX11 -lpthread -lXrandr -lXi -ldl)
set(SDL_Linkers -lSDL2 -lSDL2_mixer)
set(LibSndFileLinkers -lsndfile)
set(fftwLinkers -lfftw3 -lm)

add_compile_options(-Wall -Werror)

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
include_directories("${PROJECT_SOURCE_DIR}/external/cglm/include/")

set(CMAKE_BUILD_TYPE Debug)

add_executable(rework
    main.c
        application.h
        application.c
        renderer/renderer.c
        renderer/renderer.h
        renderer/window.c
        renderer/window.h
        external/glad/src/glad.c
        renderer/shader.c
        renderer/shader.h
        renderer/vertex_buffer.c
        renderer/vertex_buffer.h
        renderer/index_buffer.c
        renderer/index_buffer.h
        renderer/vertex_array.c
        renderer/vertex_array.h)

# Link required libraries
target_link_libraries(rework
        PRIVATE
        glfw
        ${SDL2_LIBRARIES}
        ${SDL2mixer_LIBRARIES}
        PkgConfig::FFTW
        m           # math lib for fftw
        dl          # dynamic loader, used by glad on some platforms
        pthread     # for glfw
    ${OpenGlLinkers})