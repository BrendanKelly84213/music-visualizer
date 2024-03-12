//
// Created by brendan on 2/22/24.
//

#include "VertexArray.h"
#include "VertexBuffer.h"

#include <glad/glad.h>

std::shared_ptr<VertexArray> VertexArray::create()
{
    return std::make_shared<VertexArray>();
}

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

void VertexArray::addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) const
{
    bind();
    vertexBuffer->bind();
    unsigned int offset = 0;
    auto layout = vertexBuffer->layout();
    for (unsigned i = 0; i < layout.size(); ++i) {
        auto const& attribute = layout.at(i);
        glVertexAttribPointer(i, attribute.count, layoutTypeToGLenum(attribute.type), attribute.normalized ? GL_TRUE : GL_FALSE, layout.stride(), reinterpret_cast<void const*>(offset * attribute.size));
        glEnableVertexAttribArray(i);
        offset += attribute.count;
    }
}
