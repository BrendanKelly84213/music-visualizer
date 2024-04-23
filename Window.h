//
// Created by brendan on 2/28/24.
//

#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <utility>

class Window {
public:
    Window() = default;
    Window(int width, int height, std::string&& title)
    : m_window(nullptr), m_width(width), m_height(height), m_title(std::move(title))
    {
    }

    ~Window();

    bool init();
    [[nodiscard]] GLFWwindow* ptr() const { return m_window; }
private:
    GLFWwindow* m_window{};
    int m_width {0};
    int m_height {0};
    std::string m_title;
};


#endif //WINDOW_H
