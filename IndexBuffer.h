//
// Created by brendan on 2/21/24.
//

#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H

#include <glad/glad.h>

class IndexBuffer {
public:
    IndexBuffer(const unsigned int* data, unsigned int count);
    ~IndexBuffer();
    void bind() const;
private:
    unsigned int m_id {0};
};


#endif //INDEXBUFFER_H
