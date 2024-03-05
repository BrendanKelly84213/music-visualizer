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
#include "VertexArray.h"
#include "VertexAttributes.h"
#include "SoundData.h"
#include "IndexBuffer.h"

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

    Shader shaderProgram;
    if (!shaderProgram.load("/home/brendan/dev/music-visualizer/vertex-shader.glsl", "/home/brendan/dev/music-visualizer/fragment-shader.glsl")) {
        std::cout << "Failed to load shader program\n";
        return 1;
    }

    float vertices[] = {
        1.0f,  1.0f, 0.0f,   // top right
        1.0f, -1.0, 0.0f,    // bottom right
        -1.0f, -1.0f, 0.0f,  // bottom left
        -1.0f,  1.0f, 0.0f  // top left
    };
    unsigned int indices[] = {
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    VertexArray vertexArray;
    VertexAttributes vertexAttributes;
    VertexBuffer vertexBuffer(vertices, sizeof(vertices));
    IndexBuffer indexBuffer(indices, 6);

    vertexAttributes.push<float>(3);
    vertexAttributes.enable(vertexBuffer, vertexArray);

    auto windowPtr = window.ptr();
    auto* in = fftw_alloc_complex(dataBlockSize);
    auto* out = fftw_alloc_complex(dataBlockSize);
    double lastTime = 0.0f;
    size_t numFrames = 0;
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

        std::cout << "deltaTime: " << deltaTime << '\n';
        auto samplesSinceLastFrame = samplerate * deltaTime;
        std::cout << "samplesSinceLastFrame: " << samplesSinceLastFrame << '\n';
        // Round samplesPerFrame up to the nearest power of 2
        auto power = std::ceil(std::log2(samplesSinceLastFrame));
        auto samplesPerFramePowerOf2 = static_cast<size_t>(std::pow(2, power));
        std::cout << "samplesPerFrame (rounded up to nearest power of 2): " << samplesPerFramePowerOf2 << '\n';

        std::cout << "Before: ";
        for (size_t i = 0; i < samplesPerFramePowerOf2; ++i) {
            auto dataIndex = i + numFrames * samplesPerFramePowerOf2;
            in[i][0] = data.at(dataIndex);
            in[i][1] = 0;
            std::cout << "{ real: " << in[i][0] << ", complex: " << in[i][1] << " } ";
        }
        std::cout << "\n------------------------\n";

        auto* p = fftw_plan_dft_1d(dataBlockSize, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

        double magnitudes[dataBlockSize];
        std::cout << "After: ";
        for (size_t i = 0; i < samplesPerFramePowerOf2; ++i) {
            magnitudes[i] = std::sqrt(out[i][0] * out[i][0] + out[i][1] * out[i][1]);
            std::cout << "{ real: " << out[i][0] << ", complex: " << out[i][1] << ", magnitude: " << magnitudes[i] << " } ";
        }
        std::cout << "\n------------------------\n";

        fftw_execute(p);
        fftw_destroy_plan(p);

        int width;
        int height;
        glfwGetWindowSize(windowPtr, &width, &height);
        auto resolution = glm::vec2(width, height);
        shaderProgram.setUniform("uTime", currentTime);
        shaderProgram.setUniform<glm::vec2>("uResolution", resolution);
        shaderProgram.use();
        Renderer::clear();
        Renderer::drawElements(6);

        glfwPollEvents();
        glfwSwapBuffers(windowPtr);
        numFrames++;
    }
    fftw_free(in);
    fftw_free(out);

    return 0;
}
