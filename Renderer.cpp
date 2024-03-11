//
// Created by brendan on 2/27/24.
//

#include "Renderer.h"
#include "VertexArray.h"
#include "VertexAttributes.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "RenderCommand.h"

void Renderer::drawRectangle(float rectangleWidth, float rectangleHeight, const glm::vec2& bottomLeftPosition, const glm::vec4& color)
{

    std::string vertexShaderSrc = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;

        out vec3 o_position;

        void main()
        {
            gl_Position = vec4(aPos, 1.0);
            o_position = aPos;
        }
    )";

    std::string fragmentShaderSrc = R"(
        #version 330 core
        out vec4 FragColor;
        uniform vec4 u_color;
        void main()
        {
            FragColor = u_color;
        }
    )";

    Shader shaderProgram;
    auto result = shaderProgram.loadFromRaw(vertexShaderSrc, fragmentShaderSrc);
    if (result.isError()) {
        std::cout << result.error().message() << '\n';
        return;
    }

    shaderProgram.use();
    shaderProgram.setUniform4f("u_color", color);

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

    RenderCommand::drawIndexed(6);
}

