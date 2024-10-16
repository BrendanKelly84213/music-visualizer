//
// Created by brendan on 2/27/24.
//

#include "Renderer.h"

#include <memory>
#include <glm/ext/matrix_transform.hpp>
#include "VertexArray.h"
#include "VertexLayout.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "RenderCommand.h"

std::string Renderer::s_quadVertexShaderSrc = R"(
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

std::string Renderer::s_quadFragmentShaderSrc = R"(
    #version 330 core
    out vec4 FragColor;
    uniform vec4 u_color;
    void main()
    {
        FragColor = u_color;
    }
)";

float Renderer::s_quadVertices[12] = {
    0.5f, 0.5f, 0.0f, // top right
    0.5f, -0.5f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f, // bottom left
    -0.5f, 0.5f, 0.0f // top left
};

unsigned int Renderer::s_quadIndices[6] = {
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};

std::unique_ptr<Renderer> Renderer::create()
{
    auto renderer = std::make_unique<Renderer>(Renderer());
    auto defaultShader = Shader::create();
    auto result = defaultShader->loadFromRaw(s_quadVertexShaderSrc, s_quadFragmentShaderSrc);
    if (result.isError()) {
        std::cout << result.error().message() << '\n';
        return nullptr;
    }
    renderer->m_shaders["default"] = defaultShader;
    renderer->m_vertexArray = VertexArray::create();
    if (renderer->m_vertexArray == nullptr) {
        std::cout << "vertex array is null\n";
        return nullptr;
    }
    renderer->m_vertexBuffer = VertexBuffer::create(s_quadVertices, sizeof(s_quadVertices));
    if (renderer->m_vertexBuffer == nullptr) {
        std::cout << "vertex buffer is null\n";
        return nullptr;
    }
    renderer->m_indexBuffer = IndexBuffer::create(renderer->s_quadIndices, 6);
    if (renderer->m_indexBuffer == nullptr) {
        std::cout << "vertex buffer is null\n";
        return nullptr;
    }
    return renderer;
}

void Renderer::drawQuad(const glm::vec2& dimensions, const glm::vec2 &position, const glm::vec4 &color)
{
    auto transform = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(dimensions, 0.0f));
    drawQuad(color, transform);
}

void Renderer::drawQuad(const glm::vec4& color, const glm::mat4& transform)
{
    m_shaders["default"]->use();
    m_shaders["default"]->setUniform4f("u_color", color);
    m_shaders["default"]->setUniformMat4f("u_transform", transform);

    m_vertexBuffer->setLayout({
      {VertexLayoutType::Float3, "aPos"}
    });

    m_vertexArray->addVertexBuffer(m_vertexBuffer);
}

bool Renderer::drawShaderQuad(const std::string& shaderName, const glm::mat4 &transform)
{
    if (m_shaders[shaderName] == nullptr) {
        return false;
    }

    // FIXME: Dynamic uniforms
    m_shaders[shaderName]->use();
    m_shaders[shaderName]->setUniform2f("u_resolution", {800, 600});
    m_shaders[shaderName]->setUniformMat4f("u_transform", transform);

    m_vertexBuffer->setLayout({
        {VertexLayoutType::Float3, "aPos"}
    });

    m_vertexArray->addVertexBuffer(m_vertexBuffer);
    RenderCommand::drawIndexed(6);
    return true;
}