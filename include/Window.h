//
// Created by brendan on 2/28/24.
//

#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

class Window {
public:
    Window(int width, int height, const std::string& title);
    ~Window();

    [[nodiscard]] GLFWwindow* window() const { return m_window; }
private:
    GLFWwindow* m_window;
};


#endif //WINDOW_H
