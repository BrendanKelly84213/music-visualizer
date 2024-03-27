//
// Created by brendan on 3/21/24.
//

#ifndef GUI_H
#define GUI_H


#include "Music.h"
#include "Window.h"
#include "Renderer.h"
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
    [[nodiscard]] bool renderCustomShader() const { return m_renderCustomShader; }
    [[nodiscard]] const std::string& currentShaderQuad() const { return m_currentShaderQuad; }
private:
    bool m_renderSpectrum;
    bool m_renderCustomShader;
    std::string m_currentShaderQuad;
};


#endif //GUI_H
