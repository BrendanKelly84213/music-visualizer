//
// Created by brendan on 2/27/24.
//

#include "Renderer.h"
#include "VertexArray.h"
#include "VertexAttributes.h"
#include "IndexBuffer.h"
#include "Shader.h"

void Renderer::clear(GLbitfield mask)
{
    glClear(mask);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void Renderer::drawElements(int count, const void* indices)
{
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, indices);
}

void Renderer::drawArrays(int first, int count, unsigned int mode)
{
    glDrawArrays(mode, first, count);
}

void Renderer::drawRectangle(float rectangleWidth, float rectangleHeight, glm::vec2 bottomLeftPosition)
{
    std::string vertexShaderSrc = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;

        out vec3 ourPosition;

        void main()
        {
            gl_Position = vec4(aPos, 1.0);
            ourPosition = aPos;
        }
    )";

    std::string fragmentShaderSrc = R"(
        #version 330 core
        out vec4 FragColor;
        uniform vec4 ourColor;
        void main()
        {
            FragColor = vec4(0.4, 0.0, 0.0, 1);
        }
    )";

    Shader shaderProgram;
    auto result = shaderProgram.loadFromRaw(vertexShaderSrc, fragmentShaderSrc);
    if (result.isError()) {
        std::cout << result.error().message() << '\n';
        return;
    }

    float vertices[] = {
        bottomLeftPosition.x + rectangleWidth, bottomLeftPosition.y + rectangleHeight, 0.0f, // Top right
        bottomLeftPosition.x + rectangleWidth, bottomLeftPosition.y, 0.0f, // Bottom right
        bottomLeftPosition.x, bottomLeftPosition.y, 0.0f, // Bottom left
        bottomLeftPosition.x, bottomLeftPosition.y + rectangleHeight, 0.0f, // Top left
    };

    unsigned int indices[] = {
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    VertexArray vertexArray;
    VertexAttributes vertexAttributes;
    VertexBuffer vertexBuffer(vertices, sizeof(vertices));
    IndexBuffer indexBuffer(indices, 6);

    vertexAttributes.push<float>(3);
    vertexAttributes.enable(vertexBuffer, vertexArray);

    shaderProgram.use();
    drawElements(6);
}

