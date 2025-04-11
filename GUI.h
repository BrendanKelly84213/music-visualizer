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
    GUI() { }
    ~GUI();
    void init(const Window& window, std::shared_ptr<FrameBuffer> const& frame_buffer, std::shared_ptr<Renderer> const& renderer);
    void debug();
    static void render();
    void newFrame();
    void onFrame();

    NodeEditor const& nodeEditor() const { return m_nodeEditor; }
private:
    NodeEditor m_nodeEditor;
    std::shared_ptr<FrameBuffer> m_framebuffer;
    std::shared_ptr<Renderer> m_renderer;
};

#endif //GUI_H
