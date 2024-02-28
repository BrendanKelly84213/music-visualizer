//
// Created by brendan on 2/22/24.
//

#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include <vector>
#include "VertexBuffer.h"


struct VertexAttribute {
    int size;
    unsigned int type;
    unsigned char normalized;
    int sizeOfType;
};

class VertexArray {
public:
    VertexArray();
    ~VertexArray();
    void bind() const;

    void push(int size, unsigned int type, unsigned char normalized);

    void enableVertexAttributes(VertexBuffer const&);
private:
    unsigned int m_id {0};
    unsigned int m_index {0};
    int m_stride {0};
    std::vector<VertexAttribute> m_attributes {};
};


#endif //VERTEXARRAY_H
