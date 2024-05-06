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

    m_gui.init(m_window);
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
    GUI::newFrame();
    m_gui.mainMenu();

    RenderCommand::setClearColor({0.0,0.0,0.1, 1.0});
    RenderCommand::clear();

    GUI::render();
    glfwSwapBuffers(m_window.ptr());
}




