#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <array>

#include "Window.h"
#include "Music.h"
#include "Renderer.h"
#include "SoundData.h"
#include "RenderCommand.h"
#include "FFT.h"

const unsigned int dataBlockSize = 1024;

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

    auto music = TRY(Music::create("/home/brendan/dev/my-stuff/music-visualizer/test/hoty.wav"), 1);
    auto samplerate = music->data()->info().samplerate;
    auto fft = TRY(FFT::create(dataBlockSize, music->data()), 1);

    auto renderer = Renderer::create();
    if (renderer == nullptr) {
        std::cout << "Renderer is null\n";
        return 1;
    }

    double lastTime = 0.0f;
    while (!glfwWindowShouldClose(window.ptr())) {
        auto currentTime = glfwGetTime();
        auto deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        if (glfwGetKey(window.ptr(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window.ptr(), true);
        }

        if (!Music::playing()) {
            music->play(0);
        }

        auto samplesSinceLastFrame = static_cast<size_t>(samplerate * deltaTime);
        fft->execute(samplesSinceLastFrame);

        // Render
        RenderCommand::setClearColor({0.0,0.0,0.1, 1.0});
        RenderCommand::clear();
        const size_t numSamplesShown = samplesSinceLastFrame / 2.0;
        auto rectangleWidth = 2.0 / static_cast<double>(numSamplesShown);
        for (size_t i = 0; i < numSamplesShown; i++) {
            auto rectangleHeight = fft->magnitudeAt(i) * .01;
            renderer->drawQuad({rectangleWidth, rectangleHeight}, {(static_cast<double>(i) * rectangleWidth - 1.0), 0.0}, {1.0, 0.2, 0.3, 1.0});
            RenderCommand::drawIndexed(6);
        }

        glfwSwapBuffers(window.ptr());
        glfwPollEvents();
    }

    return 0;
}
