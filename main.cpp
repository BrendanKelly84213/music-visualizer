#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <array>
#include <filesystem>
#include <algorithm>
#include <thread>
#include <glm/ext/matrix_transform.hpp>

#include "Window.h"
#include "Music.h"
#include "Renderer.h"
#include "SoundData.h"
#include "RenderCommand.h"
#include "GUI.h"
#include "Spectrum.h"
#include "imgui.h"
#include "PerlinNoise.h"
#include "config.h"

namespace fs = std::filesystem;

unsigned int dataBlockSize = 1024;
float frameRate = 0;

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

    GUI gui(window);
    auto music = TRY(Music::create(dataBlockSize), 1);
    auto renderer = Renderer::create();
    if (renderer == nullptr) {
        std::cout << "Renderer is null\n";
        return 1;
    }
    auto perlinNoise = PerlinNoise::create();
    if (perlinNoise == nullptr) {
        std::cout << "PerlinNoise is is null\n";
        return 1;
    }

    int samplerate = 0;
    double lastTime = 0.0f;
    float color[4] = {0.5, 0.0, 0.0, 1.0 };
    while (!glfwWindowShouldClose(window.ptr())) {
        auto currentTime = glfwGetTime();
        auto deltaTime = currentTime - lastTime;
        frameRate = 1.0f / deltaTime;
        lastTime = currentTime;
        glfwPollEvents();
        GUI::newFrame();
        gui.mainMenu(music, renderer, frameRate);

        if (glfwGetKey(window.ptr(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window.ptr(), true);
        }

        if (!music->loaded() && !music->songPath().empty()) {
            if (music->load()) {
                samplerate = music->info().samplerate;
            }
        }

        if (!Music::playing() && music->loaded()) {
            music->play(0);
        }

        auto samplesSinceLastFrame = static_cast<size_t>(samplerate * deltaTime);
        music->executeFFT(samplesSinceLastFrame);

        // Render
        RenderCommand::setClearColor({0.0,0.0,0.1, 1.0});
        RenderCommand::clear();

        if (gui.renderCustomShader()) {
            if (!renderer->drawShaderQuad("custom", (float)currentTime)) {
                gui.toggleCustomShader();
            }
        }

        if (gui.renderSpectrum()) {
            Spectrum::render(samplesSinceLastFrame, music, renderer, glm::vec4(color[0], color[1], color[2], color[3]));
        }

        GUI::render();
        glfwSwapBuffers(window.ptr());
    }

    return 0;
}
