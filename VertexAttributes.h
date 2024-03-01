//
// Created by brendan on 3/1/24.
//

#ifndef VERTEXATTRIBUTES_H
#define VERTEXATTRIBUTES_H


#include "VertexBuffer.h"
#include "VertexArray.h"

struct VertexAttribute {
    int size;
    unsigned int type;
    unsigned char normalized;
    int sizeOfType;
};

class VertexAttributes {
public:
    void push(int size, unsigned int type, unsigned char normalized);
    void enable(const VertexBuffer &vertexBuffer, const VertexArray &vertexArray);
private:
    std::vector<VertexAttribute> m_attributes {};
    unsigned int m_index {0};
    int m_stride {0};
};


#endif //VERTEXATTRIBUTES_H
