//
// Created by brendan on 2/21/24.
//

#include "IndexBuffer.h"

std::shared_ptr<IndexBuffer> IndexBuffer::create(unsigned int *data, unsigned int count)
{
    auto indexBuffer = std::make_shared<IndexBuffer>();
    glGenBuffers(1, &indexBuffer->m_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer->m_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
    return indexBuffer;
}

IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &m_id);
}

void IndexBuffer::bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
}

