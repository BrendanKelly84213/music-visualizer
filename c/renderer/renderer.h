#ifndef RENDERER_H
#define RENDERER_H

#include "index_buffer.h"
#include "shader.h"
#include "vertex_array.h"
#include "vertex_buffer.h"
#include "cglm/cglm.h"

typedef struct {
    shader_id_t quad_shader;
    vertex_array_id_t vertex_array;
    vertex_buffer_t* vertex_buffer;
    index_buffer_id_t index_buffer;
} renderer_t;

renderer_t* renderer_create();
void renderer_destroy(renderer_t* renderer);
void renderer_draw_quad(renderer_t* renderer, vec4 color, vec3 dimensions, vec3 position);
void renderer_draw_transform_quad(renderer_t* renderer, vec4 color, mat4 transform);

#endif
