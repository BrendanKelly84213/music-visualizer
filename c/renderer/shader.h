//
// Created by brendan on 5/3/25.
//

#ifndef SHADER_H
#define SHADER_H

typedef struct {
    unsigned int id;
} shader_t;

shader_t* shader_load_from_raw(char const* vertex_shader, char const* fragment_shader);
void shader_destroy(shader_t* shader);

#endif // SHADER_H
