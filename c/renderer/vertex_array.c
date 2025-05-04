//
// Created by brendan on 5/3/25.
//

#include "vertex_array.h"
#include <malloc.h>

vertex_array_t* va_create()
{
    vertex_array_t* va = (vertex_array_t*)malloc(sizeof(vertex_array_t));
    if (!va) {
        return NULL;
    }

    glGenVertexArrays(1, &va->id);
    return va;
}

void va_destroy(vertex_array_t* va)
{
    if (va) {
        glDeleteVertexArrays(1, &va->id);
        free(va);
    }
}

void va_bind(vertex_array_t* va)
{
    glBindVertexArray(va->id);
}

void va_unbind(vertex_array_t* va)
{
    glBindVertexArray(0);
}

void va_add_vb(vertex_array_t* va, vertex_buffer_t* vb)
{
    va_bind(va);
    vb_bind(vb);
    if (!vb->layout) {
        return; // No layout set
    }

    for (size_t i = 0; i < vb->layout->count; ++i) {
        vertex_attribute_t* attr = &vb->layout->attributes[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, attr->count, attr->type, attr->normalized, vb->layout->stride, (void*)(attr->offset));
    }
}
