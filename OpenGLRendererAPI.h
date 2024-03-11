//
// Created by brendan on 3/11/24.
//

#ifndef OPENGLRENDERERAPI_H
#define OPENGLRENDERERAPI_H

#include <glad/glad.h>
#include <glm/vec4.hpp>
#include "RendererAPI.h"

class OpenGLRendererAPI : public RendererAPI {
public:
    virtual void setClearColor(const glm::vec4& color) override;
    virtual void clear() override;
    virtual void drawIndexed(int count, const void* indices) override;
};


#endif //OPENGLRENDERERAPI_H
