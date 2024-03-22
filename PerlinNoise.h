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
    void updateTime(float time)
    {
        m_shader->use();
        m_shader->setUniform1f("u_time", time);
    }
    void render(double currentTime, const std::shared_ptr<Music>& music, const std::shared_ptr<Renderer>& renderer);
    [[nodiscard]] std::shared_ptr<Shader> shader() const { return m_shader; }
    [[nodiscard]] std::string vertexShaderPath() const { return m_vertexShaderPath; }
    [[nodiscard]] std::string fragmentShaderPath() const { return m_fragmentShaderPath; }
private:
    std::shared_ptr<Shader> m_shader;
    std::string m_vertexShaderPath;
    std::string m_fragmentShaderPath;
};


#endif //PERLINNOISE_H
