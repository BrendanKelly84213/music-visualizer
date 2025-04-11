//
// Created by brendan on 4/23/24.
//

#include "Application.h"

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

    m_framebuffer = FrameBuffer::create();
    if (m_framebuffer == nullptr) {
        std::cout << "FrameBuffer is null\n";
        return false;
    }
    m_renderer = Renderer::create();
    if (m_renderer == nullptr) {
        std::cout << "Renderer is null\n";
        return false;
    }
    m_gui.init(m_window, m_framebuffer, m_renderer);
    return true;
}

int Application::run()
{
    while (!glfwWindowShouldClose(m_window.ptr())) {
        getInputs();
        render();
    }
    return 0;
}

void Application::getInputs()
{
    glfwPollEvents();
    if (glfwGetKey(m_window.ptr(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(m_window.ptr(), true);
    }
}

void Application::render()
{
    m_gui.newFrame();

    RenderCommand::setClearColor({0.0,0.0,0.1, 1.0});
    RenderCommand::clear();

    GUI::render();

    glfwSwapBuffers(m_window.ptr());
}
