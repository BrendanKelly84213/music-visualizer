cmake_minimum_required(VERSION 3.16)
project(rework)

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

add_executable(rework
    main.c
        application.h
        application.c
        renderer.c
        renderer.h
        window.c
        window.h
        external/glad/src/glad.c)

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