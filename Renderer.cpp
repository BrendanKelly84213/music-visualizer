//
// Created by brendan on 2/27/24.
//

#include "Renderer.h"

void Renderer::clear(GLbitfield mask)
{
    glClear(mask);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void Renderer::drawElements(int count, const void* indices)
{
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, indices);
}

void Renderer::drawArrays(int first, int count)
{
    glDrawArrays(GL_TRIANGLES, first, count);
}
