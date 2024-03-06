//
// Created by brendan on 2/27/24.
//

#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <glm/vec2.hpp>

class Renderer {
public:
    Renderer() = default;
    static void clear(GLbitfield mask = GL_COLOR_BUFFER_BIT);
    static void drawElements(int count, const void* indices = nullptr);
    static void drawArrays(int first, int count, unsigned int mode = GL_TRIANGLES);
    // Normalized values
    void drawRectangle(float rectangleWidth, float rectangleHeight, glm::vec2 bottomLeftPosition);
private:

};


#endif //RENDERER_H
