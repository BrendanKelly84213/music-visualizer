//
// Created by brendan on 3/22/24.
//

#include "PerlinNoise.h"
#include "config.h"

std::shared_ptr<PerlinNoise> PerlinNoise::create()
{
    auto instance = std::make_shared<PerlinNoise>();
    instance->m_shader = Shader::create();

    std::string root = LOCAL_PATH("assets/shaders/");
    instance->m_vertexShaderPath = root + "vertex-shader.glsl";
    instance->m_fragmentShaderPath = root + "smoke-fragment-shader.glsl";
    return instance;
}

