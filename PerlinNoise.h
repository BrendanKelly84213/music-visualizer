//
// Created by brendan on 3/22/24.
//

#ifndef PERLINNOISE_H
#define PERLINNOISE_H


#include "Shader.h"
#include "Music.h"
#include "Renderer.h"
class PerlinNoise {
public:
    static std::shared_ptr<PerlinNoise> create();
    [[nodiscard]] std::shared_ptr<Shader> shader() const { return m_shader; }
    [[nodiscard]] std::string vertexShaderPath() const { return m_vertexShaderPath; }
    [[nodiscard]] std::string fragmentShaderPath() const { return m_fragmentShaderPath; }
private:
    std::shared_ptr<Shader> m_shader;
    std::string m_vertexShaderPath;
    std::string m_fragmentShaderPath;
};


#endif //PERLINNOISE_H
