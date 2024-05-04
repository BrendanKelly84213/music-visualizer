//
// Created by brendan on 4/23/24.
//

#include "Application.h"

namespace fs = std::filesystem;

float frameRate = 0;

bool Application::init()
{
    if (!m_window.init()) {
        std::cout << "Failed to initialize window" << '\n';
        return false;
    }

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    m_gui.init(m_window);

    m_music = TRY(Music::create(), false);
    m_renderer = Renderer::create();
    if (m_renderer == nullptr) {
        std::cout << "Renderer is null\n";
        return false;
    }

    return true;
}

int Application::run()
{
    int samplerate = 0;
    double lastTime = 0.0f;
    float color[4] = {0.5, 0.0, 0.0, 1.0 };
    while (!glfwWindowShouldClose(m_window.ptr())) {
        auto currentTime = glfwGetTime();
        auto deltaTime = currentTime - lastTime;
        frameRate = 1.0f / deltaTime;
        lastTime = currentTime;
        glfwPollEvents();
        GUI::newFrame();
        m_gui.mainMenu(m_music, m_renderer, frameRate);

        if (glfwGetKey(m_window.ptr(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(m_window.ptr(), true);
        }

        if (!m_music->loaded() && !m_music->songPath().empty()) {
            if (m_music->load()) {
                samplerate = m_music->info().samplerate;
            }
        }

        if (!Music::playing() && m_music->loaded()) {
            m_music->play(0);
        }

        auto samplesSinceLastFrame = static_cast<size_t>(samplerate * deltaTime);
        m_music->executeFFT(samplesSinceLastFrame);

        // Render
        RenderCommand::setClearColor({0.0,0.0,0.1, 1.0});
        RenderCommand::clear();

        if (m_gui.shouldRenderCustomShader()) {
            if (!m_renderer->drawShaderQuad("custom")) {
                m_gui.toggleCustomShader();
            }
        }

        if (m_gui.renderSpectrum()) {
            Spectrum::render(samplesSinceLastFrame, m_music, m_renderer, glm::vec4(color[0], color[1], color[2], color[3]));
        }

        GUI::render();
        glfwSwapBuffers(m_window.ptr());
    }
    return 0;
}




