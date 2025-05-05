//
// Created by brendan on 5/3/25.
//

#include "shader.h"
#include "glad/glad.h"
#include <malloc.h>
#include <stdio.h>

static int shader_compile(char const* source, unsigned int type)
{
    int shader_id = glCreateShader(type);
    glShaderSource(shader_id, 1, &source, NULL);
    glCompileShader(shader_id);

    int success;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info_log[512];
        glGetShaderInfoLog(shader_id, 512, NULL, info_log);
        fprintf(stderr, "ERROR::SHADER::COMPILATION_FAILED\n%s\n", info_log);
        return -1;
    }

    return shader_id;
}

static int shader_link_program(int vertex_shader_id, int fragment_shader_id)
{
    int program_id = glCreateProgram();
    glAttachShader(program_id, vertex_shader_id);
    glAttachShader(program_id, fragment_shader_id);
    glLinkProgram(program_id);

    int success;
    glGetProgramiv(program_id, GL_LINK_STATUS, &success);
    if (!success) {
        char info_log[512];
        glGetProgramInfoLog(program_id, 512, NULL, info_log);
        fprintf(stderr, "ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", info_log);
        return -1;
    }

    return program_id;
}

shader_id_t shader_load_from_raw(char const* vertex_shader, char const* fragment_shader)
{
    shader_id_t id = 0;

    int vertex_shader_id = shader_compile(vertex_shader, GL_VERTEX_SHADER);
    if (vertex_shader_id == -1) {
        fprintf(stderr, "Failed to compile vertex shader\n");
        return 0;
    }

    int fragment_shader_id = shader_compile(fragment_shader, GL_FRAGMENT_SHADER);
    if (fragment_shader_id == -1) {
        fprintf(stderr, "Failed to compile fragment shader\n");
        return 0;
    }

    id = shader_link_program(vertex_shader_id, fragment_shader_id);
    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);

    return id;
}

void shader_use(shader_id_t shader_id)
{
    glUseProgram(shader_id);
}

void shader_set_uniform4f(shader_id_t shader_id, char const* name, vec4 value)
{
    int location = glGetUniformLocation(shader_id, name);
    if (location != -1) {
        glUniform4fv(location, 1, value);
    } else {
        fprintf(stderr, "Warning: uniform '%s' not found in shader program\n", name);
    }
}

void shader_set_uniform_mat4f(shader_id_t shader_id, char const* name, mat4 value)
{
    int location = glGetUniformLocation(shader_id, name);
    if (location != -1) {
        glUniformMatrix4fv(location, 1, GL_FALSE, (const GLfloat*)value);
    } else {
        fprintf(stderr, "Warning: uniform '%s' not found in shader program\n", name);
    }
}