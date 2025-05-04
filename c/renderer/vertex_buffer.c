//
// Created by brendan on 5/3/25.
//

#include "vertex_buffer.h"
#include <malloc.h>

void layout_calculate_stride(vertex_layout_t* layout)
{
    for (size_t i = 0; i < layout->count; ++i) {
        layout->stride += layout->attributes[i].size;
    }
}

vertex_buffer_t* vb_create(void const* data, unsigned int size)
{
    vertex_buffer_t* vb = (vertex_buffer_t*)malloc(sizeof(vertex_buffer_t));
    if (!vb) {
        return NULL; // Memory allocation failed
    }

    glGenBuffers(1, &vb->id);
    glBindBuffer(GL_ARRAY_BUFFER, vb->id);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    vb->size = size;
    vb->layout = NULL;

    return vb;
}

void vb_destroy(vertex_buffer_t* vb)
{
    if (vb) {
        if (vb->layout) {
            free(vb->layout->attributes);
            free(vb->layout);
        }
        glDeleteBuffers(1, &vb->id);
        free(vb);
    }
}

void vb_set_layout(vertex_buffer_t* vb, vertex_layout_t* layout)
{
    vb->layout = layout;
    layout_calculate_stride(layout);
}

void vb_bind(vertex_buffer_t* vb)
{
    glBindBuffer(GL_ARRAY_BUFFER, vb->id);
}

void vb_unbind(vertex_buffer_t* vb)
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}