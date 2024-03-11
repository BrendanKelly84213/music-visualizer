//
// Created by brendan on 3/10/24.
//

#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <glm/glm.hpp>
#include <memory>
#include "VertexLayout.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Renderer.h"

class Rectangle {
public:
    static std::shared_ptr<Rectangle> create(float width, float height, const glm::vec2& position, const glm::vec4& color);
    void draw()
    {
        m_shaderProgram.use();
        // Seems silly to have all these temporary objects get created just to be destroyed immediately
        VertexArray vertexArray;
        VertexLayout vertexAttributes;
        VertexBuffer vertexBuffer(m_vertices, sizeof(m_vertices));
        IndexBuffer indexBuffer(indices, 6);
        vertexAttributes.push<float>(3);
        vertexAttributes.enable(vertexBuffer, vertexArray);

        Renderer::drawElements(6);
    }

    Shader shaderProgram() const { return m_shaderProgram; }
    float width() const { return m_width; }
    float height() const { return m_height; }
    glm::vec2 position() const { return m_position; }
private:
    Rectangle(Shader&& shaderProgram, float width, float height, const glm::vec2& position, const glm::vec4& color)
        : m_shaderProgram(shaderProgram), m_width(width), m_height(height), m_position(position), m_color(color)
    {}

    Shader m_shaderProgram;
    float m_width;
    float m_height;
    glm::vec2 m_position {};
    glm::vec4 m_color {};

    const float m_vertices[12] = {
        1.0f, 1.0f, 0.0f, // Top right
        1.0f, -1.0f, 0.0f, // Bottom right
        -1.0f, -1.0f, 0.0f, // Bottom left
        -1.0f, 1.0f, 0.0f, // Top left
    };

    unsigned int indices[6] = {
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };
};


#endif //RECTANGLE_H
