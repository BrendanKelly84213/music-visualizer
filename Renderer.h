//
// Created by brendan on 2/27/24.
//

#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

class Renderer {
public:
    Renderer() = default;
    // Normalized values
    void drawRectangle(float rectangleWidth, float rectangleHeight, const glm::vec2& bottomLeftPosition, const glm::vec4& color);
private:
};


#endif //RENDERER_H
