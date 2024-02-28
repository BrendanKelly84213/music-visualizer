//
// Created by brendan on 2/22/24.
//

#include "VertexArray.h"
#include <glad/glad.h>

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &m_id);
    glBindVertexArray(m_id);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_id);
}

void VertexArray::bind() const
{
    glBindVertexArray(m_id);
}

void VertexArray::push(int size, unsigned int type, unsigned char normalized)
{
    int sizeOfType = 0;
    switch (type) {
        case GL_FLOAT:
            m_stride += size * sizeof(float);
            sizeOfType = sizeof(GLfloat);
            break;
        default: break;
    }

    m_attributes.push_back({ size, type, normalized, sizeOfType });
    m_index++;
}

void VertexArray::enableVertexAttributes(VertexBuffer const& vertexBuffer)
{
    bind();
    vertexBuffer.bind();
    unsigned int offset = 0;
    for (unsigned i = 0; i < m_attributes.size(); ++i) {
        auto const& attribute = m_attributes[i];
        glVertexAttribPointer(i, attribute.size, attribute.type, attribute.normalized, m_stride, reinterpret_cast<void const*>(offset * attribute.sizeOfType));
        glEnableVertexAttribArray(i);
        offset += attribute.size;
    }
}
