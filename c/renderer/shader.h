//
// Created by brendan on 5/3/25.
//

#ifndef SHADER_H
#define SHADER_H

#include "cglm/cglm.h"

typedef unsigned int shader_id_t;

shader_id_t shader_load_from_raw(char const* vertex_shader, char const* fragment_shader);
void shader_use(shader_id_t shader_id);
void shader_set_uniform4f(shader_id_t shader_id, char const* name, vec4 value);
void shader_set_uniform_mat4f(shader_id_t shader_id, char const* name, mat4 value);

#endif // SHADER_H
