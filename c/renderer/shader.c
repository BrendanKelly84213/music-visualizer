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

shader_t* shader_load_from_raw(char const* vertex_shader, char const* fragment_shader)
{
    shader_t* shader = malloc(sizeof(shader_t));

    int vertex_shader_id = shader_compile(vertex_shader, GL_VERTEX_SHADER);
    if (vertex_shader_id == -1) {
        fprintf(stderr, "Failed to compile vertex shader\n");
        return NULL;
    }

    int fragment_shader_id = shader_compile(fragment_shader, GL_FRAGMENT_SHADER);
    if (fragment_shader_id == -1) {
        fprintf(stderr, "Failed to compile fragment shader\n");
        return NULL;
    }

    shader->id = shader_link_program(vertex_shader_id, fragment_shader_id);
    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);

    return shader;
}
void shader_destroy(shader_t* shader)
{
    if (shader != NULL) {
        glDeleteProgram(shader->id);
        free(shader);
    }
}
