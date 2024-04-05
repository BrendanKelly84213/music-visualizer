//
// Created by brendan on 3/21/24.
//

#ifndef GUI_H
#define GUI_H


#include "Music.h"
#include "Window.h"
#include "Renderer.h"

struct Uniform {
    std::string type;
    std::string name;
    std::string value;
    // FIXME: Should only contain one value member
    std::variant<float, glm::vec2, glm::vec3, glm::vec4, glm::mat4> hardValue;
};

class ShaderEditor {
public:
    void draw(const std::shared_ptr<Renderer>& renderer);
    [[nodiscard]] bool shouldRenderShader() const { return m_shouldRenderShader; }
private:
    std::vector<Uniform> m_uniforms;
    bool m_shouldRenderShader {false};
    std::string m_compileMessage;
    std::string m_vertexShaderPath;
    std::string m_fragmentShaderPath;
    Shader* m_shader;
};

class GUI {
public:
    GUI(const Window& window);
    ~GUI();
    void mainMenu(const std::shared_ptr<Music> &music,
                  const std::shared_ptr<Renderer> &renderer,
                  float frameRate);
    void debug();
    static void render();
    static void newFrame();
    // FIXME: Temporary! We should be able  to access a variable number of different visuals in the future. So hardcoding presets like this won't work!
    void toggleRenderSpectrum() { m_renderSpectrum = !m_renderSpectrum; }
    void toggleCustomShader() { m_renderCustomShader = !m_renderCustomShader; }
    void setCurrentShaderQuad(const std::string& name) { m_currentShaderQuad = name; }
    [[nodiscard]] bool renderSpectrum() const { return m_renderSpectrum; }
    [[nodiscard]] bool shouldRenderCustomShader() const { return m_shaderEditor.shouldRenderShader(); }
    [[nodiscard]] const std::string& currentShaderQuad() const { return m_currentShaderQuad; }
private:
    bool m_renderSpectrum;
    bool m_renderCustomShader;
    std::string m_currentShaderQuad;
    // FIXME: We probably want to be able torender more than one shader at a time
    ShaderEditor m_shaderEditor;
};


#endif //GUI_H
