#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <array>
#include <filesystem>
#include <algorithm>
#include <thread>

#include "Window.h"
#include "Music.h"
#include "Renderer.h"
#include "SoundData.h"
#include "RenderCommand.h"
#include "GUI.h"
#include "Spectrum.h"
#include "imgui.h"

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

    int samplerate = 0;
    double lastTime = 0.0f;

    while (!glfwWindowShouldClose(window.ptr())) {
        auto currentTime = glfwGetTime();
        auto deltaTime = currentTime - lastTime;
        frameRate = 1.0f / deltaTime;
        lastTime = currentTime;
        glfwPollEvents();
        GUI::newFrame();
        gui.mainMenu(music, frameRate);

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
        if (gui.renderSpectrum()) {
            Spectrum::render(samplesSinceLastFrame, music, renderer);
        }
        GUI::render();
        glfwSwapBuffers(window.ptr());
    }

    return 0;
}
