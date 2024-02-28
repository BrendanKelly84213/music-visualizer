//
// Created by brendan on 2/27/24.
//

#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>

class Renderer {
public:
    Renderer() = default;
    static void clear(GLbitfield mask = GL_COLOR_BUFFER_BIT);
    static void drawElements(int count, const void* indices = nullptr);
private:
};


#endif //RENDERER_H
