//
// Created by brendan on 3/1/24.
//

#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <glm/glm.hpp>
#include <atomic>

class Shader {
public:
    ~Shader();
    void use() const;
    bool load(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

    template<typename T>
    void setUniform(const std::string& name, T value)
    {
        auto location = glGetUniformLocation(m_id, name.c_str());
        if constexpr (std::is_same_v<float, T> || std::is_same_v<double, T>) {
            glUniform1f(location, value);
        } else if constexpr (std::is_same_v<glm::vec2, T>) {
            glUniform2f(location, value.x, value.y);
        }
    }

private:
    unsigned int m_id {0};
    char m_infoLog[512] {};

    int compileShader(const std::string& source, unsigned int typeFlag);
    int linkProgram(unsigned int vertexShader, unsigned int fragmentShader);
};


#endif //SHADER_H
