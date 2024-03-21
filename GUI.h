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
    static void render();
    static void newFrame();
    void toggleRenderSpectrum() { m_renderSpectrum = !m_renderSpectrum; }
    [[nodiscard]] bool renderSpectrum() const { return m_renderSpectrum; }
private:
    bool m_renderSpectrum;
};


#endif //GUI_H
