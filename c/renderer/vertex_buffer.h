//
// Created by brendan on 5/3/25.
//

#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include "glad/glad.h"
#include <stddef.h>

typedef struct {
    GLenum type;
    GLint count;
    GLboolean normalized;
    GLsizei size;
    size_t offset;
} vertex_attribute_t;

typedef struct {
    vertex_attribute_t* attributes;
    GLint count;
    GLsizei stride;
} vertex_layout_t;

typedef struct {
    unsigned int id;
    unsigned int size;
    vertex_layout_t* layout;
} vertex_buffer_t;

void layout_calculate_stride(vertex_layout_t* layout);

vertex_buffer_t* vb_create(void const* data, unsigned int size);
void vb_destroy(vertex_buffer_t* vb);
void vb_set_layout(vertex_buffer_t* vb, vertex_layout_t* layout);
void vb_bind(vertex_buffer_t* vb);
void vb_unbind(vertex_buffer_t* vb);

#endif // VERTEX_BUFFER_H
