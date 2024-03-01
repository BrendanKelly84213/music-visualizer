//
// Created by brendan on 2/22/24.
//

#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include <vector>
#include "VertexBuffer.h"

class VertexArray {
public:
    VertexArray();
    ~VertexArray();
    void bind() const;
private:
    unsigned int m_id {0};
};

#endif //VERTEXARRAY_H
