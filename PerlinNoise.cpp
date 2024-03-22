//
// Created by brendan on 3/22/24.
//

#include "PerlinNoise.h"

std::shared_ptr<PerlinNoise> PerlinNoise::create()
{
    auto instance = std::make_shared<PerlinNoise>();
    instance->m_shader = Shader::create();
    std::string root = "/home/brendan/dev/my-stuff/music-visualizer/assets/shaders/";
    instance->m_vertexShaderPath = root + "vertex-shader.glsl";
    instance->m_fragmentShaderPath = root + "smoke-fragment-shader.glsl";
    return instance;
}

void PerlinNoise::render(double currentTime,
                         const std::shared_ptr<Music> &music,
                         const std::shared_ptr<Renderer> &renderer)
{
    // FIXME: This is temporary, we should probably keep a hashmap or otherwise of shaders depending on what is getting drawn instead of overwriting like this since we definitely are going to want more than two shader in the future


    renderer->drawShaderQuad(currentTime);
}
