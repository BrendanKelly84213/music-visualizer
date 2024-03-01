//
// Created by brendan on 3/1/24.
//

#ifndef VERTEXATTRIBUTES_H
#define VERTEXATTRIBUTES_H


#include <iostream>
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
    // Just playing around with templates and different implementations of this API
    // This is a workaround for templating with
    // template<typename T>
    // VertexAttributes::push(...);
    // template<>
    // VertexAttributes::push<float>() etc... which does not work in class scope using gcc
    // Has the benefit of a slightly nicer API to use, less readable implementation
    template<typename T>
    void push(int size)
    {
        m_stride += size * sizeof(T);
        if constexpr (std::is_same_v<T, float>) {
            m_attributes.push_back({ size, GL_FLOAT, GL_FALSE, sizeof(GLfloat)});
        } else if constexpr (std::is_same_v<T, unsigned int>) {
            m_attributes.push_back({ size, GL_UNSIGNED_INT, GL_FALSE, sizeof(GLuint)});
        }
    }
    void push(int size, unsigned int type, unsigned char normalized);
    void enable(const VertexBuffer &vertexBuffer, const VertexArray &vertexArray);
private:
    std::vector<VertexAttribute> m_attributes {};
    int m_stride {0};
};


#endif //VERTEXATTRIBUTES_H
