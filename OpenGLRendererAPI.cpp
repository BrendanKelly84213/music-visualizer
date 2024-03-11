//
// Created by brendan on 3/11/24.
//

#include "OpenGLRendererAPI.h"

void OpenGLRendererAPI::setClearColor(const glm::vec4& color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}

void OpenGLRendererAPI::clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRendererAPI::drawIndexed(int count, const void* indices)
{
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, indices);
}
