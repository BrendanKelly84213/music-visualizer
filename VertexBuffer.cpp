//
// Created by brendan on 2/21/24.
//

#include "VertexBuffer.h"

std::shared_ptr<VertexBuffer> VertexBuffer::create(const void *data, unsigned int size)
{
    auto vertexBuffer= std::make_shared<VertexBuffer>();
    glGenBuffers(1, &vertexBuffer->m_id);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->m_id);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    return vertexBuffer;
}

void VertexBuffer::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
}
