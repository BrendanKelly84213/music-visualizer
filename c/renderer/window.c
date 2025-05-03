#include "window.h"
#include <malloc.h>

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

window_t* window_create(int width, int height, const char* title)
{
    window_t* window = malloc(sizeof(window_t));
    if (window == NULL) {
        return NULL;
    }

    window->width = width;
    window->height = height;
    window->title = title;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window->glfw_window = glfwCreateWindow(window->width, window->height, window->title, NULL, NULL);
    if (window->glfw_window == NULL) {
        free(window);
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window->glfw_window);
    glfwSetFramebufferSizeCallback(window->glfw_window, framebuffer_size_callback);

    return window;
}

void window_destroy(window_t* window)
{
    if (window != NULL) {
        glfwDestroyWindow(window->glfw_window);
        free(window);
    }
}