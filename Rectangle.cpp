//
// Created by brendan on 3/10/24.
//

#include <string>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "Rectangle.h"
#include "Shader.h"
#include "VertexArray.h"

std::shared_ptr<Rectangle> Rectangle::create(float width, float height, const glm::vec2& position, const glm::vec4 &color)
{
    std::string vertexShaderSrc = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;

        out vec3 o_position;
        uniform mat4 u_transform;

        void main()
        {
            gl_Position = u_transform * vec4(aPos, 1.0);
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
        return nullptr;
    }

    auto transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(position.x, position.y, 0.0f));
    transform = glm::scale(transform, glm::vec3(width + 1, height, 1.0f));

    shaderProgram.use();
    shaderProgram.setUniformMat4f("u_transform", transform);
    shaderProgram.setUniform4f("u_color", color);

    return std::make_shared<Rectangle>(Rectangle(std::move(shaderProgram), width, height, position, color));
}
