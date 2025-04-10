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
    m_renderer = Renderer::create();
    if (m_renderer == nullptr) {
        std::cout << "Renderer is null\n";
        return false;
    }
    m_gui.init(m_window, m_framebuffer);
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
    m_framebuffer->bind();

    RenderCommand::setClearColor({0.0,0.0,0.1, 1.0});
    RenderCommand::clear();

    // FIXME: Temporary debugging thing. In the future outputs can should represent anything graphical...
    auto color = m_gui.nodeEditor().output();
    m_renderer->drawQuad({0.5, 0.5}, {0, 0}, glm::vec4{color & 0x000000FF,color & 0x0000FF00,color & 0x00FF0000, 1});
    RenderCommand::drawIndexed(6);
    m_framebuffer->unbind();

    RenderCommand::setClearColor({0.0,0.0,0.0, 1.0});
    RenderCommand::clear();
    GUI::render();

    glfwSwapBuffers(m_window.ptr());
}
