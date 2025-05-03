#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>
typedef struct
{
    int width;
    int height;
    const char* title;
    GLFWwindow* glfw_window;
} window_t;

window_t* window_create(int width, int height, const char* title);

#endif
