//
// Created by brendan on 3/21/24.
//

#ifndef GUI_H
#define GUI_H


#include <memory>

#include "FrameBuffer.h"
#include "Music.h"
#include "NodeEditor.h"
#include "Renderer.h"
#include "Window.h"
#include "imgui.h"

class GUI {
public:
    ~GUI();
    void init(const Window& window, std::shared_ptr<FrameBuffer> const& frame_buffer);
    void debug();
    static void render();
    void newFrame();
    void onFrame();
private:
    NodeEditor m_nodeGraph;
    std::shared_ptr<FrameBuffer> m_framebuffer;
};

#endif //GUI_H
