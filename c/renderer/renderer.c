#include "renderer.h"
#include <malloc.h>
#include <string.h>

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
    if (renderer->quad_shader == 0) {
        printf("Failed to load shader\n");
        return NULL;
    }

    renderer->vertex_array = va_create();
    if (renderer->vertex_array == 0) {
        printf("Failed to create vertex array\n");
        return NULL;
    }

    renderer->vertex_buffer = vb_create(quad_vertices, sizeof(quad_vertices));
    if (renderer->vertex_buffer == NULL) {
        printf("Failed to create vertex buffer\n");
        va_destroy(renderer->vertex_array);
        return NULL;
    }

    renderer->index_buffer = ib_create(quad_indices, 6);
    if (renderer->index_buffer == 0) {
        printf("Failed to create index buffer\n");
        vb_destroy(renderer->vertex_buffer);
        va_destroy(renderer->vertex_array);
        return NULL;
    }

    return renderer;
}

void renderer_destroy(renderer_t* renderer)
{
    if (renderer != NULL) {
        free(renderer);
    }
}

void renderer_draw_quad(renderer_t* renderer, vec4 color, vec3 dimensions, vec3 position)
{
    mat4 transform = GLM_MAT4_IDENTITY_INIT;
    glm_translate(transform, position);
    glm_scale(transform, dimensions);
    renderer_draw_transform_quad(renderer, color, transform);
}

void renderer_draw_transform_quad(renderer_t* renderer, vec4 color, mat4 transform)
{
    shader_use(renderer->quad_shader);
    shader_set_uniform4f(renderer->quad_shader, "u_color", color);
    shader_set_uniform_mat4f(renderer->quad_shader, "u_transform", transform);

    renderer->vertex_buffer->layout = (vertex_layout_t*)malloc(sizeof(vertex_layout_t));
    if (renderer->vertex_buffer->layout == NULL) {
        return;
    }

    renderer->vertex_buffer->layout->count = 1;
    renderer->vertex_buffer->layout->attributes = (vertex_attribute_t*)malloc(sizeof(vertex_attribute_t) * renderer->vertex_buffer->layout->count);
    renderer->vertex_buffer->layout->attributes[0] = (vertex_attribute_t) {
        .type = GL_FLOAT,
        .count = 3,
        .normalized = GL_FALSE,
        .size = sizeof(float) * 3,
        .offset = 0
    };

    layout_calculate_stride(renderer->vertex_buffer->layout);
    va_add_vb(renderer->vertex_array, renderer->vertex_buffer);
}
