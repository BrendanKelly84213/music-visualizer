#ifndef RENDERER_H
#define RENDERER_H

#include "index_buffer.h"
#include "shader.h"
#include "vertex_array.h"
#include "vertex_buffer.h"

typedef struct {
    shader_t* quad_shader;
    vertex_array_t* vertex_array;
    vertex_buffer_t* vertex_buffer;
    index_buffer_t* index_buffer;
} renderer_t;

renderer_t* renderer_create();
void renderer_destroy(renderer_t* renderer);
void renderer_draw_quad(renderer_t* renderer, float x, float y, float width, float height);

#endif
