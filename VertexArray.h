//
// Created by brendan on 2/22/24.
//

#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include <vector>
#include <memory>
#include "VertexLayout.h"

class VertexBuffer;

class VertexArray {
public:
    static std::shared_ptr<VertexArray> create();

    VertexArray();
    ~VertexArray();
    void bind() const;

    void addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) const;
private:
    unsigned int m_id {0};
};

#endif //VERTEXARRAY_H
