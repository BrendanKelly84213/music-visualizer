//
// Created by brendan on 5/3/25.
//

#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include "vertex_buffer.h"
typedef struct {
    unsigned int id;
} vertex_array_t;

vertex_array_t* va_create(void);
void va_destroy(vertex_array_t* va);
void va_bind(vertex_array_t* va);
void va_unbind(vertex_array_t* va);
void va_add_vb(vertex_array_t* va, vertex_buffer_t* vb);

#endif // VERTEX_ARRAY_H
