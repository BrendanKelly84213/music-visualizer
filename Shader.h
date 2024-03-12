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

class Shader {
public:
    static std::shared_ptr<Shader> create()
    {
        return std::make_shared<Shader>();
    }

    ~Shader();
    void use() const;
    int compileShader(const std::string& source, unsigned int typeFlag);
    int linkProgram(unsigned int vertexShader, unsigned int fragmentShader);
    Result<unsigned int> load(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
    Result<unsigned int> loadFromRaw(const std::string &vertexShaderCode, const std::string &fragmentShaderCode);

    void setUniform4f(const std::string& name, float x, float y, float z, float w) const;
    void setUniform4f(const std::string& name, const glm::vec4& value) const
    {
        setUniform4f(name, value.x, value.y, value.z, value.w);
    }
    void setUniformMat4f(const std::string& name, const glm::mat4& value) const;

    unsigned int id() const { return m_id; }

private:
    unsigned int m_id {0};
    char m_infoLog[512] {};
};


#endif //SHADER_H
