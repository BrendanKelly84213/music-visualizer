//
// Created by brendan on 3/1/24.
//

#include <sstream>
#include <fstream>
#include <iostream>
#include <glad/glad.h>
#include "Shader.h"

// TODO: unsigned int -> enum
int Shader::compileShader(const std::string& source, unsigned int typeFlag)
{
    int  success;
    int shader = glCreateShader(typeFlag);
    auto sourceCStr = source.c_str();
    glShaderSource(shader, 1, &sourceCStr, nullptr);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, m_infoLog);
        return -1;
    }

    return shader;
}

int Shader::linkProgram(unsigned int vertexShader,  unsigned int fragmentShader)
{
    int success;
    int program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(program, 512, nullptr, m_infoLog);
        return -1;
    }

    return program;
}

static std::string readFileToString(const std::string& filepath)
{
    std::ifstream file(filepath);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

bool Shader::load(const std::string &vertexShaderPath, const std::string &fragmentShaderPath)
{
    std::string vertexShaderCode = readFileToString(vertexShaderPath);
    std::string fragmentShaderCode = readFileToString(fragmentShaderPath);

    int vertexShader = compileShader(vertexShaderCode, GL_VERTEX_SHADER);
    if (vertexShader < 0) {
        std::cout << "Failed to compile vertexShader: " << m_infoLog << '\n';
        return false;
    }

    int fragmentShader = compileShader(fragmentShaderCode, GL_FRAGMENT_SHADER);
    if (fragmentShader < 0) {
        std::cout << "Failed to compile fragmentShader: " << m_infoLog << '\n';
        return false;
    }

    m_id = linkProgram(vertexShader, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return true;
}

void Shader::use() const
{
    glUseProgram(m_id);
}

Shader::~Shader()
{
    glDeleteProgram(m_id);
}
