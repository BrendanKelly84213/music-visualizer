//
// Created by brendan on 3/1/24.
//

#ifndef VERTEXLAYOUT_H
#define VERTEXLAYOUT_H


#include <iostream>
#include <utility>
#include <vector>
#include <glad/glad.h>

class VertexArray;

enum class VertexLayoutType {
    None = 0,
    Float3,
};

static int layoutTypeToGLenum(VertexLayoutType type)
{
    switch (type) {
        case VertexLayoutType::Float3: return GL_FLOAT;
        default: break;
    }
    std::cout << "VertexLayout :: typeToGLenum :: Unrecognized type" << '\n';
    return 0;
}

static int layoutTypeToCount(VertexLayoutType type)
{
    switch (type) {
        case VertexLayoutType::Float3: return 3;
        default: break;
    }
    std::cout << "VertexLayout :: typeToCount :: Unrecognized type" << '\n';
    return 0;
}

static int layoutTypeToSize(VertexLayoutType type)
{
    switch (type) {
        case VertexLayoutType::Float3: return 3 * 4;
        default: break;
    }
    std::cout << "VertexLayout :: typeToSize :: Unrecognized type" << '\n';
    return 0;
}

struct VertexAttribute {
    VertexLayoutType type;
    std::string name;
    int size;
    bool normalized;
    int count;

    VertexAttribute(VertexLayoutType p_type, std::string p_name)
    : type(p_type), name(std::move(p_name)), size(layoutTypeToSize(p_type)), normalized(false), count(layoutTypeToCount(p_type))
    {}
};

class VertexLayout {
public:
    VertexLayout(const std::initializer_list<VertexAttribute>& attributes)
    : m_attributes(attributes)
    {
        calculateStride();
    }

    void calculateStride()
    {
        m_stride = 0;
        for (auto const& attribute : m_attributes) {
            m_stride += layoutTypeToSize(attribute.type);
        }
    }

    int stride() const { return m_stride; }
    size_t size() const { return m_attributes.size(); }
    VertexAttribute const& at(size_t i) const { return m_attributes.at(i); }
private:
    std::vector<VertexAttribute> m_attributes {};
    int m_stride {0};
};


#endif //VERTEXLAYOUT_H
