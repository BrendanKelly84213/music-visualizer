//
// Created by brendan on 2/21/24.
//

#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H


#include <glad/glad.h>
#include <memory>
#include "VertexLayout.h"


class VertexBuffer {
public:
    static std::shared_ptr<VertexBuffer> create(const void* data, unsigned int size);
    ~VertexBuffer()
    {
        glDeleteBuffers(1, &m_id);
    }

    void bind() const;
    void unbind() const;

    void setLayout(const VertexLayout& vertexLayout) { m_layout = vertexLayout; }
    const VertexLayout& layout() const { return m_layout; }

private:
    unsigned int m_id {0};
    VertexLayout m_layout {};
};


#endif //VERTEXBUFFER_H
