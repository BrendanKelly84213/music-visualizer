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

namespace fs = std::filesystem;

unsigned int dataBlockSize = 1024;
float frameRate = 0;

float clamp(float x, float lowerlimit = 0.0f, float upperlimit = 1.0f) {
    if (x < lowerlimit) return lowerlimit;
    if (x > upperlimit) return upperlimit;
    return x;
}

float smoothstep (float edge0, float edge1, float x) {
    // Scale, and clamp x to 0..1 range
    x = clamp((x - edge0) / (edge1 - edge0));

    return x * x * (3.0f - 2.0f * x);
}

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
    double speed = 0.1;
    float scaleFactor = 0.0;
    float left = 0.0;
    float right = 1.0;
    bool useSmoothStep = false;
    float color[4];
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

        auto smoothstepAverage = smoothstep(left, right, music->fftAverage());
        if (useSmoothStep)
            speed += smoothstepAverage;
        else
            speed += music->fftAverage();

        ImGui::SliderFloat("speed", &scaleFactor, 0.0, 1.0);
        ImGui::SliderFloat("left", &left, 0.0, 10.0);
        ImGui::SliderFloat("right", &right, 0.0, 10.0);
        ImGui::Text("average: %.3f", music->fftAverage());
        ImGui::Text("smoothstep average: %.3f", smoothstepAverage);
        ImGui::Text("speed: %.3f", speed);
        ImGui::Checkbox("use smoothstep", &useSmoothStep);
        ImGui::ColorPicker4("Color", color);

        if (gui.renderNoise()) {
            if (!renderer->usingCustomShader()) {
                renderer->loadCustomShader(perlinNoise->vertexShaderPath(), perlinNoise->fragmentShaderPath());
            }
            auto transform = glm::scale(glm::mat4(1.0f), glm::vec3(2.0, 2.0, 0.0));
            renderer->drawShaderQuad(speed * scaleFactor, transform);
            RenderCommand::drawIndexed(6);
        }

        if (gui.renderSpectrum()) {
            Spectrum::render(samplesSinceLastFrame, music, renderer, glm::vec4(color[0], color[1], color[2], color[3]));
        }

        GUI::render();
        glfwSwapBuffers(window.ptr());
    }

    return 0;
}
