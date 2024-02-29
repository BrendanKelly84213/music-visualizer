#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <memory>
#include <csignal>
#include "include/Window.h"
#include "Music.h"

int main()
{
    Window window(800, 600, "Hello Window");
    if (!window.init()) {
        std::cout << "Failed to initialize window" << '\n';
        return 1;
    }

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return 1;
    }

    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cout << "Failed to initialize SDL " << SDL_GetError() << '\n';
        return 1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cout << "Failed to initialize SDL_mixer " << Mix_GetError() << '\n';
        return 1;
    }

    Music music;
    if (!music.load("/home/brendan/dev/music-visualizer/hoty.wav")) {
        std::cout << "Failed to load music file " << Mix_GetError() << '\n';
        return 1;
    }

    auto windowPtr = window.ptr();
    while (!glfwWindowShouldClose(windowPtr)) {
        if (glfwGetKey(windowPtr, GLFW_KEY_ESCAPE) == GLFW_KEY_DOWN) {
            glfwSetWindowShouldClose(windowPtr, true);
        }

        if (!Mix_PlayingMusic()) {
            Mix_PlayMusic(music.ptr(), -1);
        }

        glfwPollEvents();
    }

    return 0;
}
