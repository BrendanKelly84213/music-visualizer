//
// Created by brendan on 2/27/24.
//

#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <memory>
#include "Shader.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"

class Renderer {
public:
    static std::shared_ptr<Renderer> create();
    void drawQuad(const glm::vec2& dimensions, const glm::vec2& position, const glm::vec4& color);
    void drawQuad( const glm::vec4 &color, const glm::mat4 &transform = glm::mat4(1.0f));

    [[nodiscard]] std::shared_ptr<IndexBuffer> indexBuffer() const { return m_indexBuffer; }
private:
    std::shared_ptr<Shader> m_quadShader {};
    std::shared_ptr<VertexArray> m_vertexArray {};
    std::shared_ptr<VertexBuffer> m_vertexBuffer {};
    std::shared_ptr<IndexBuffer> m_indexBuffer {};

    // FIXME: Temporary!
    static float s_quadVertices[12];
    static unsigned int s_quadIndices[6];

    static std::string s_quadVertexShaderSrc;
    static std::string s_quadFragmentShaderSrc;
};


#endif //RENDERER_H
