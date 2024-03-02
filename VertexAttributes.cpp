//
// Created by brendan on 3/1/24.
//

#include "VertexAttributes.h"

void VertexAttributes::push(int size, unsigned int type, unsigned char normalized)
{
    int sizeOfType = 0;
    switch (type) {
        case GL_FLOAT:
            m_stride += size * sizeof(float);
            sizeOfType = sizeof(GLfloat);
            break;
        case GL_UNSIGNED_INT:
            m_stride += size * sizeof(unsigned int);
            sizeOfType = sizeof(GLuint);
            break;
        default: break;
    }

    m_attributes.push_back({ size, type, normalized, sizeOfType });
}

void VertexAttributes::enable(VertexBuffer const& vertexBuffer, VertexArray const& vertexArray)
{
    vertexArray.bind();
    vertexBuffer.bind();
    unsigned int offset = 0;
    for (unsigned i = 0; i < m_attributes.size(); ++i) {
        auto const& attribute = m_attributes[i];
        glVertexAttribPointer(i, attribute.size, attribute.type, attribute.normalized, m_stride, reinterpret_cast<void const*>(offset * attribute.sizeOfType));
        glEnableVertexAttribArray(i);
        offset += attribute.size;
    }
}
