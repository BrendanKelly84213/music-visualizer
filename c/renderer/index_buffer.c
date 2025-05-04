//
// Created by brendan on 5/3/25.
//

#include <glad/glad.h>
#include "index_buffer.h"
#include <stdio.h>

index_buffer_id_t ib_create(unsigned int* indices, unsigned int count)
{
    index_buffer_id_t id = 0;
    glGenBuffers(1, &id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW);
    return id;
}

void ib_destroy(index_buffer_id_t ib)
{
    glDeleteBuffers(1, &ib);
}

void ib_bind(index_buffer_id_t ib)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
}

void ib_unbind(index_buffer_id_t ib)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}