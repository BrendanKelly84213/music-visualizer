#include "renderer.h"
#include <malloc.h>

static char const* quad_vertex_shader_src = "#version 330 core\n"
                                            "layout (location = 0) in vec3 aPos;\n"
                                            "\n"
                                            "out vec3 o_position;\n"
                                            "uniform mat4 u_transform;\n"
                                            "\n"
                                            "void main()\n"
                                            "{\n"
                                            "    gl_Position = u_transform * vec4(aPos, 1.0);\n"
                                            "    o_position = aPos;\n"
                                            "}\n";

static char const* quad_fragment_shader_src = "#version 330 core\n"
                                              "out vec4 FragColor;\n"
                                              "uniform vec4 u_color;\n"
                                              "void main()\n"
                                              "{\n"
                                              "    FragColor = u_color;\n"
                                              "}\n";

static float const quad_vertices[12] = {
    1.0f, 1.0f, 0.0f,   // top right
    1.0f, -1.0f, 0.0f,  // bottom right
    -1.0f, -1.0f, 0.0f, // bottom left
    -1.0f, 1.0f, 0.0f   // top left
};

static unsigned int const quad_indices[6] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};

renderer_t* renderer_create(void)
{
    renderer_t* renderer = malloc(sizeof(renderer_t));
    if (renderer == NULL) {
        return NULL;
    }

    renderer->quad_shader = shader_load_from_raw(quad_vertex_shader_src, quad_fragment_shader_src);
    if (renderer->quad_shader == NULL) {
        printf("Failed to load shader\n");
        return NULL;
    }

    return renderer;
}

void renderer_destroy(renderer_t* renderer)
{
    if (renderer != NULL) {
        // Clean up OpenGL context and other resources here
        shader_destroy(renderer->quad_shader);
        free(renderer);
    }
}
void renderer_draw_quad(renderer_t* renderer, float x, float y, float width, float height)
{
}
