#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <sndfile.h>
#include <array>
#include <fftw3.h>

#include "Window.h"
#include "Music.h"
#include "Renderer.h"
#include "Shader.h"
#include "SoundData.h"

const unsigned int dataBlockSize = 16384;

#define TRY(expression, returnValue)                                        \
    ({                                                                      \
        auto&& temporary = expression;                                      \
        if (std::holds_alternative<Error>(std::move(temporary))) {          \
          std::cout << "Error: " << std::get<Error>(temporary).m_message;   \
          return returnValue;                                               \
        }                                                                   \
        std::get<SoundData>(std::move(temporary));                          \
    })                                                                      \

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

    // NOTE: Experimental
    auto data = TRY(SoundData::create("/home/brendan/dev/music-visualizer/hoty.wav"), 1);
    auto samplerate = data.info().samplerate;


    auto windowPtr = window.ptr();
    auto* in = fftw_alloc_complex(dataBlockSize);
    auto* out = fftw_alloc_complex(dataBlockSize);
    double lastTime = 0.0f;
    size_t numFrames = 0;
    size_t dataIndex = 0;
    Renderer renderer;
    while (!glfwWindowShouldClose(windowPtr)) {
        auto currentTime = glfwGetTime();
        auto deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        if (glfwGetKey(windowPtr, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(windowPtr, true);
        }

        if (!Mix_PlayingMusic()) {
            Mix_PlayMusic(music.ptr(), -1);
        }

        auto samplesSinceLastFrame = static_cast<size_t>(samplerate * deltaTime);
        // Round samplesPerFrame down to the nearest power of 2
        auto power = std::floor(std::log2(samplesSinceLastFrame));
        auto samplesPerFramePowerOf2 = static_cast<size_t>(std::pow(2, power));

        for (size_t i = 0; i < samplesPerFramePowerOf2; ++i) {
            in[i][0] = data.at(dataIndex);
            in[i][1] = 0;
            dataIndex++;
        }

        fftw_plan_s* p = fftw_plan_dft_1d(static_cast<int>(samplesPerFramePowerOf2), in, out, FFTW_FORWARD, FFTW_ESTIMATE);
        fftw_execute(p);

        const size_t numSamplesShown = 100;
        double magnitudes[numSamplesShown];
        for (size_t i = 0; i < numSamplesShown; ++i) {
            magnitudes[i] = std::sqrt(out[i][0] * out[i][0] + out[i][1] * out[i][1]);
        }

        fftw_destroy_plan(p);

        int width;
        int height;
        glfwGetWindowSize(windowPtr, &width, &height);
        auto resolution = glm::vec2(width, height);
        Renderer::clear();
        auto rectangleWidth = 1 / static_cast<double>(numSamplesShown);
        for (size_t i = 0; i < numSamplesShown; ++i) {
            auto rectangleHeight = magnitudes[i] * 0.01;
            auto bottomLeftPosition = glm::vec2((static_cast<double>(i) * rectangleWidth - 0.5) * 2.0f, -1);
            renderer.drawRectangle(rectangleWidth, rectangleHeight, bottomLeftPosition);
        }

        glfwPollEvents();
        glfwSwapBuffers(windowPtr);
        numFrames++;
    }
    fftw_free(in);
    fftw_free(out);

    return 0;
}
