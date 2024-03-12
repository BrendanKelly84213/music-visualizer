//
// Created by brendan on 3/11/24.
//

#include <iostream>
#include <memory>
#include "RendererAPI.h"
#include "OpenGLRendererAPI.h"

RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

std::unique_ptr<RendererAPI> RendererAPI::create()
{
    if (s_API == RendererAPI::API::OpenGL) {
        return std::make_unique<OpenGLRendererAPI>();
    }
    std::cout << "Unsupported rendering API" << '\n';
    return nullptr;
}