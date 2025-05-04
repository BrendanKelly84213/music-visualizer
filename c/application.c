//
// Created by brendan on 5/2/25.
//

#include <glad/glad.h>
#include <stdio.h>

#include "application.h"
#include "renderer/renderer.h"
#include "renderer/window.h"

typedef struct {
    window_t* window;
    renderer_t* renderer;
} application_context_t;

static application_context_t g_application_context;

int application_init()
{
    g_application_context.window = window_create(500, 500, "Hello World");
    if (g_application_context.window == NULL) {
        printf("Failed to initialize window\n");
        return 0;
    }

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize GLAD\n");
        return 0;
    }

    g_application_context.renderer = renderer_create();
    if (g_application_context.renderer == NULL) {
        printf("Renderer is null\n");
        return 0;
    }

    return 1;
}

static void application_get_inputs()
{
    glfwPollEvents();
    if (glfwGetKey(g_application_context.window->glfw_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(g_application_context.window->glfw_window, 1);
    }
}

static void application_render()
{
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, indices);
    renderer_draw_quad(g_application_context.renderer,
        (vec4) { 0.0f, 0.0f, 1.0f, 1.0f },
        (vec3) { 1.0f, 1.0f, 1.0f },
        (vec3) { 0.0f, 0.0f, 0.0f });

    glfwSwapBuffers(g_application_context.window->glfw_window);
}

void application_run()
{
    while (!glfwWindowShouldClose(g_application_context.window->glfw_window)) {
        application_get_inputs();
        application_render();
    }

    window_destroy(g_application_context.window);
    renderer_destroy(g_application_context.renderer);
    glfwTerminate();
}
