//
// Created by brendan on 3/1/24.
//

#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <glm/glm.hpp>
#include <atomic>
#include <memory>
#include "Error.h"

class Shader : public std::enable_shared_from_this<Shader> {
public:
    static std::shared_ptr<Shader> create()
    {
        return std::make_shared<Shader>();
    }

    ~Shader();
    void use() const;
    int compileShader(const std::string& source, unsigned int typeFlag);
    int linkProgram(unsigned int vertexShader, unsigned int fragmentShader);
    Result<std::shared_ptr<Shader>> load(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
    Result<std::shared_ptr<Shader>> loadDefaultVertex(const std::string& vertexShaderSource, const std::string& fragmentShaderPath);
    Result<std::shared_ptr<Shader>> loadFromRaw(const std::string &vertexShaderCode, const std::string &fragmentShaderCode);

    void setUniform1f(const std::string& name, float value) const;
    void setUniform2f(const std::string& name, float x, float y) const;
    void setUniform2f(const std::string& name, const glm::vec2& value) const
    {
        setUniform2f(name, value.x, value.y);
    }
    void setUniform4f(const std::string& name, float x, float y, float z, float w) const;
    void setUniform4f(const std::string& name, const glm::vec4& value) const
    {
        setUniform4f(name, value.x, value.y, value.z, value.w);
    }
    void setUniformMat4f(const std::string& name, const glm::mat4& value) const;

    [[nodiscard]] unsigned int id() const { return m_id; }
    [[nodiscard]] bool loaded() const { return m_loaded; }
private:
    unsigned int m_id {0};
    char m_infoLog[512] {};
    bool m_loaded {false};
};


#endif //SHADER_H
