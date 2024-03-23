//
// Created by brendan on 3/21/24.
//

#ifndef GUI_H
#define GUI_H


#include "Music.h"
#include "Window.h"
class GUI {
public:
    GUI(const Window& window);
    ~GUI();
    void mainMenu(const std::shared_ptr<Music>& music, float frameRate);
    void debug();
    static void render();
    static void newFrame();
    // FIXME: Temporary! We should be able  to access a variable number of different visuals in the future. So hardcoding presets like this won't work!
    void toggleRenderSpectrum() { m_renderSpectrum = !m_renderSpectrum; }
    void toggleNoise() { m_renderNoise = !m_renderNoise; }
    [[nodiscard]] bool renderSpectrum() const { return m_renderSpectrum; }
    [[nodiscard]] bool renderNoise() const { return m_renderNoise; }
private:
    bool m_renderSpectrum;
    bool m_renderNoise;
};


#endif //GUI_H
