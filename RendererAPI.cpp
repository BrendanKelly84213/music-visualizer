//
// Created by brendan on 3/11/24.
//

#include <iostream>
#include "RendererAPI.h"
#include "OpenGLRendererAPI.h"

RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

RendererAPI* RendererAPI::create()
{
    if (s_API == RendererAPI::API::OpenGL) {
        return new OpenGLRendererAPI;
    }
    std::cout << "Unsupported rendering API" << '\n';
    return nullptr;
}