//
// Created by brendan on 3/11/24.
//

#ifndef RENDERERAPI_H
#define RENDERERAPI_H


#include <glm/vec4.hpp>
class RendererAPI {
public:
    enum class API {
        None = 0,
        OpenGL
    };

    static RendererAPI* create();

    virtual void setClearColor(const glm::vec4& color) = 0;
    virtual void clear() = 0;
    virtual void drawIndexed(int count, const void* indices) = 0;
private:
    static RendererAPI::API s_API;
};


#endif //RENDERERAPI_H
