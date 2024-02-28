#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "include/Window.h"

int main()
{
    Window window(800, 600, "window");

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    auto glfwWindowPtr = window.window();
    while (!glfwWindowShouldClose(glfwWindowPtr)) {
        if (glfwGetKey(glfwWindowPtr, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window.window(), true);
        }
        glfwPollEvents();
    }
    return 0;
}
