//
// Created by brendan on 5/3/25.
//

#include "vertex_array.h"
#include <malloc.h>

vertex_array_id_t va_create()
{
    vertex_array_id_t id = 0;
    glGenVertexArrays(1, &id);
    glBindVertexArray(id);
    return id;
}

void va_destroy(vertex_array_id_t va_id)
{
    if (va_id) {
        glDeleteVertexArrays(1, &va_id);
    }
}

void va_bind(vertex_array_id_t va_id)
{
    glBindVertexArray(va_id);
}

void va_unbind(vertex_array_id_t va)
{
    glBindVertexArray(0);
}

void va_add_vb(vertex_array_id_t va, vertex_buffer_t* vb)
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
