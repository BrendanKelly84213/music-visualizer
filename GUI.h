//
// Created by brendan on 3/21/24.
//

#ifndef GUI_H
#define GUI_H


#include <memory>

#include "Music.h"
#include "Window.h"
#include "Renderer.h"
#include "imgui.h"
#include "NodeGraph.h"
#include "FrameBuffer.h"

class GUI {
public:
    ~GUI();
    void init(const Window& window, std::shared_ptr<FrameBuffer> const& frame_buffer);
    void debug();
    static void render();
    void newFrame();
    void onFrame();
private:
    NodeGraph m_nodeGraph;
    std::shared_ptr<FrameBuffer> m_framebuffer;
};

#endif //GUI_H
