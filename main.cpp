#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <SDL2/SDL_mixer.h>
#include <array>
#include <fftw3.h>

#include "Window.h"
#include "Music.h"
#include "Renderer.h"
#include "SoundData.h"
#include "RenderCommand.h"

const unsigned int dataBlockSize = 4096;

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

    auto music = TRY(Music::create("/home/brendan/dev/my-stuff/music-visualizer/test/20Hz to 20kHz (Human Audio Spectrum).wav"), 1);
    auto samplerate = music->data()->info().samplerate;

    auto in = fftw_alloc_complex(dataBlockSize);
    auto out = fftw_alloc_complex(dataBlockSize);
    double lastTime = 0.0f;
    size_t dataIndex = 0;
    auto renderer = Renderer::create();
    if (renderer == nullptr) {
        std::cout << "Renderer is null\n";
        return 1;
    }
    auto p = fftw_plan_dft_1d(static_cast<int>(dataBlockSize), in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    while (!glfwWindowShouldClose(window.ptr())) {
        auto currentTime = glfwGetTime();
        auto deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        if (glfwGetKey(window.ptr(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window.ptr(), true);
        }

        if (!Music::playing()) {
            music->play();
        }

        // Collect sound data for fft
        auto samplesSinceLastFrame = static_cast<size_t>(samplerate * deltaTime);
        dataIndex += samplesSinceLastFrame;
        for (size_t i = 0; i < dataBlockSize && dataIndex < music->data()->count(); ++i) {
            in[i][0] = music->data()->at(dataIndex);
            in[i][1] = 0;
            dataIndex++;
        }

        fftw_execute(p);

        // Render
        RenderCommand::setClearColor({0.0,0.0,0.1, 1.0});
        RenderCommand::clear();
        const size_t numSamplesShown = dataBlockSize;
        auto rectangleWidth = 2.0f / static_cast<double>(numSamplesShown);
        auto magnitude = [&](size_t i) { return std::sqrt(out[i][0] * out[i][0] + out[i][1] * out[i][1]); };
        for (size_t i = 0; i < numSamplesShown; ++i) {
            auto rectangleHeight = magnitude(i) * .1;
            renderer->drawQuad({rectangleWidth, rectangleHeight}, {(static_cast<double>(i) * rectangleWidth - 1.0), 0.0}, {1.0, 0.2, 0.3, 1.0});
            RenderCommand::drawIndexed(6);
        }

        glfwSwapBuffers(window.ptr());
        glfwPollEvents();
    }
    Mix_CloseAudio();
    fftw_destroy_plan(p);
    fftw_free(in);
    fftw_free(out);

    return 0;
}
