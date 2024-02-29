#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "include/Window.h"

int main()
{
    Window window(800, 600, "window");
    window.init();

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    auto windowPtr = window.ptr();
    while (!glfwWindowShouldClose(windowPtr)) {
        if (glfwGetKey(windowPtr, GLFW_KEY_ESCAPE) == GLFW_KEY_DOWN) {
            glfwSetWindowShouldClose(windowPtr, true);
        }
        glfwPollEvents();
    }
    return 0;
}
