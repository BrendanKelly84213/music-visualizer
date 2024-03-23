//
// Created by brendan on 3/1/24.
//

#include <sstream>
#include <fstream>
#include <iostream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

Shader::~Shader()
{
    glDeleteProgram(m_id);
}

void Shader::use() const
{
    glUseProgram(m_id);
}

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
    if(!success) {
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

Result<unsigned int> Shader::load(const std::string &vertexShaderPath, const std::string &fragmentShaderPath)
{
    std::string vertexShaderCode = readFileToString(vertexShaderPath);
    std::string fragmentShaderCode = readFileToString(fragmentShaderPath);

    return loadFromRaw(vertexShaderCode, fragmentShaderCode);
}

Result<unsigned int> Shader::loadFromRaw(const std::string& vertexShaderCode, const std::string& fragmentShaderCode)
{
    int vertexShader = compileShader(vertexShaderCode, GL_VERTEX_SHADER);
    if (vertexShader < 0) {
        return Error("Failed to compile vertexShader: " + std::string(m_infoLog));
    }

    int fragmentShader = compileShader(fragmentShaderCode, GL_FRAGMENT_SHADER);
    if (fragmentShader < 0) {
        return Error("Failed to compile fragmentShader: " + std::string(m_infoLog));
    }

    m_id = linkProgram(vertexShader, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    m_loaded = true;

    return m_id;
}

void Shader::setUniform1f(const std::string &name, float value) const
{
    auto location = glGetUniformLocation(m_id, name.c_str());
    glUniform1f(location, value);
}

void Shader::setUniform2f(const std::string &name, float x, float y) const
{
    auto location = glGetUniformLocation(m_id, name.c_str());
    glUniform2f(location, x, y);
}

void Shader::setUniform4f(const std::string& name, float x, float y, float z, float w) const
{
    auto location = glGetUniformLocation(m_id, name.c_str());
    glUniform4f(location, x, y, z, w);
}

void Shader::setUniformMat4f(const std::string &name, const glm::mat4 &value) const
{
    auto location = glGetUniformLocation(m_id, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}



