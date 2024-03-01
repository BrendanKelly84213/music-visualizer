#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "Window.h"
#include "Music.h"
#include "Renderer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexAttributes.h"

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

    Shader shaderProgram;
    if (!shaderProgram.load("/home/brendan/dev/music-visualizer/vertex-shader.glsl", "/home/brendan/dev/music-visualizer/fragment-shader.glsl")) {
        std::cout << "Failed to load shader program\n";
        return 1;
    }

    float vertices[] = {
        -0.50f, -0.25f, 0.0f, 0.3f, 0.4f, 0.1f,
        0.00f, -0.25f, 0.0f, 0.3f, 0.3f, 0.2f,
        -0.25f,  0.25f, 0.0f, 0.1f, 0.4f, 0.5f,
    };

    VertexArray vertexArray;
    VertexAttributes vertexAttributes;
    VertexBuffer vertexBuffer(vertices, sizeof(vertices));

    vertexAttributes.push<float>(3);
    vertexAttributes.push<float>(3);
    vertexAttributes.enable(vertexBuffer, vertexArray);

    auto windowPtr = window.ptr();
    while (!glfwWindowShouldClose(windowPtr)) {
        if (glfwGetKey(windowPtr, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(windowPtr, true);
        }

        if (!Mix_PlayingMusic()) {
            Mix_PlayMusic(music.ptr(), -1);
        }

        Renderer::clear();

        vertexArray.bind();
        shaderProgram.use();
        Renderer::drawArrays(0, 3);

        glfwPollEvents();
        glfwSwapBuffers(windowPtr);
    }

    return 0;
}
