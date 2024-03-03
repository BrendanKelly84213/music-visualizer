#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <sndfile.h>
#include <memory>
#include <array>
#include "Window.h"
#include "Music.h"
#include "Renderer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexAttributes.h"
#include "SoundData.h"
#include "IndexBuffer.h"

#define TRY(expression, returnValue)                                        \
    ({                                                                      \
        auto&& temporary = expression;                                       \
        if (std::holds_alternative<Error>(std::move(temporary))) {          \
          std::cout << "Error: " << std::get<Error>(temporary).m_message;   \
          return returnValue;                                               \
        }                                                                   \
        std::get<SoundData>(std::move(temporary));                          \
    })                                                                      \

struct point {
    float x;
    float y;
};

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
        1.0f,  1.0f, 0.0f,  // top right
        1.0f, -1.0, 0.0f,  // bottom right
        -1.0f, -1.0f, 0.0f,  // bottom left
        -1.0f,  1.0f, 0.0f   // top left
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    std::vector<point> points;
    for (size_t i = 0; i < data.count(); ++i) {
        float x = static_cast<float>(i) / (static_cast<float>(data.count()) / 500.f) - 1.f;
        points.push_back({ x, data.at(i) / 2.f});
    }

    VertexArray vertexArray;
    VertexAttributes vertexAttributes;
    VertexBuffer vertexBuffer(points.data(), points.size() * sizeof(point));

    vertexAttributes.push<float>(2);
    vertexAttributes.enable(vertexBuffer, vertexArray);

    auto windowPtr = window.ptr();

    auto lastTime = glfwGetTime();
    float scale_x = 1.0f;
    float offset_x = 0.0f;
    while (!glfwWindowShouldClose(windowPtr)) {
        auto currentTime = glfwGetTime();
        if (glfwGetKey(windowPtr, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(windowPtr, true);
        } else if (glfwGetKey(windowPtr, GLFW_KEY_UP)) {
            scale_x *= 1.5;
        } else if (glfwGetKey(windowPtr, GLFW_KEY_DOWN)) {
            scale_x /= 1.5;
        } else if (glfwGetKey(windowPtr, GLFW_KEY_LEFT)) {
            offset_x += 1.0;
        } else if (glfwGetKey(windowPtr, GLFW_KEY_RIGHT)) {
            offset_x -= 1.0;
        }

        if (!Mix_PlayingMusic()) {
            Mix_PlayMusic(music.ptr(), -1);
        }

        auto i = static_cast<size_t>(currentTime * samplerate);
        int width;
        int height;
        glfwGetWindowSize(windowPtr, &width, &height);
        auto resolution = glm::vec2(width, height);
        shaderProgram.setUniform("uTime", currentTime);
        shaderProgram.setUniform<float>("uAmplitude", data.at(i));
        shaderProgram.setUniform<glm::vec2>("uResolution", resolution);
        shaderProgram.setUniform("offset_x", offset_x);
        shaderProgram.setUniform("scale_x", scale_x);
        vertexArray.bind();
        shaderProgram.use();
        Renderer::clear();
        Renderer::drawArrays(0, points.size(), GL_LINE_STRIP);

        glfwPollEvents();
        glfwSwapBuffers(windowPtr);
    }

    return 0;
}
