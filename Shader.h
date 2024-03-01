//
// Created by brendan on 3/1/24.
//

#ifndef SHADER_H
#define SHADER_H

#include <string>

class Shader {
public:
    ~Shader();
    void use() const;
    bool load(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
private:
    unsigned int m_id {0};
    char m_infoLog[512] {};

    int compileShader(const std::string& source, unsigned int typeFlag);
    int linkProgram(unsigned int vertexShader, unsigned int fragmentShader);
};


#endif //SHADER_H
