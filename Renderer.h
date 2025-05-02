//
// Created by brendan on 2/27/24.
//

#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <memory>
#include <unordered_map>
#include "Shader.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"

class Renderer {
public:
    static std::unique_ptr<Renderer> create();
    void drawQuad(const glm::vec2& dimensions, const glm::vec2& position, const glm::vec4& color);
    void drawQuad(const glm::vec4 &color, const glm::mat4 &transform = glm::mat4(1.0f));
    bool drawShaderQuad(const std::string& shaderName, const glm::mat4& transform = glm::mat4(1.0f));
    Result<std::shared_ptr<Shader>> loadShader(const std::string& name, const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
    {
        m_shaders[name] = Shader::create();
        return m_shaders[name]->load(vertexShaderPath, fragmentShaderPath);
    }
    Result<std::shared_ptr<Shader>> loadShader(const std::string& name, const std::string& fragmentShaderPath)
    {
        m_shaders[name] = Shader::create();
        return m_shaders[name]->loadDefaultVertex(s_quadVertexShaderSrc, fragmentShaderPath);
    }

    [[nodiscard]] std::shared_ptr<IndexBuffer> indexBuffer() const { return m_indexBuffer; }
    bool shaderLoaded(const std::string& name)
    {
        if (m_shaders.find(name) != m_shaders.end()) {
            if (m_shaders.at(name) == nullptr)
                return false;
            return m_shaders.at(name)->loaded();
        }
        return false;
    }

    std::shared_ptr<VertexBuffer> vertexBuffer() const { return m_vertexBuffer; }
    std::shared_ptr<VertexArray> vertexArray() const { return m_vertexArray; }

private:
    std::unordered_map<std::string, std::shared_ptr<Shader>> m_shaders;
    std::shared_ptr<VertexArray> m_vertexArray {};
    std::shared_ptr<VertexBuffer> m_vertexBuffer {};
    std::shared_ptr<IndexBuffer> m_indexBuffer {};

    static float s_quadVertices[12];
    static unsigned int s_quadIndices[6];

    static std::string s_quadVertexShaderSrc;
    static std::string s_quadFragmentShaderSrc;
};


#endif //RENDERER_H
