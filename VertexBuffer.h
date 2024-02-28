//
// Created by brendan on 2/21/24.
//

#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H


#include <glad/glad.h>
class VertexBuffer {
public:
    VertexBuffer(const void* data, unsigned int size);
    ~VertexBuffer()
    {
        glDeleteBuffers(1, &m_id);
    }
    void bind() const;
    void unbind() const;
private:
    unsigned int m_id {0};
};


#endif //VERTEXBUFFER_H
