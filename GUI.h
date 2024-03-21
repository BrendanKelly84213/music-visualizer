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
    static void mainMenu(const std::shared_ptr<Music>& music, float frameRate);
    static void render();
    static void newFrame();
private:
};


#endif //GUI_H
