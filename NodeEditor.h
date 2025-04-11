//
// Created by brendan on 5/4/24.
//

#ifndef NODEEDITOR_H
#define NODEEDITOR_H

#include "FrameBuffer.h"
#include "Graph.h"
#include "Renderer.h"
#include "imnodes.h"
#include <memory>
#include <vector>

enum NodeType {
    Value,
    Time,
    Add,
    Sin,
    Output,
    Debug,
    NoiseShader,
    StaticValue,
    ShaderProgram,
};

struct Node {
    NodeType type;
    float value {};
};

class NodeEditor {
public:
    NodeEditor();
    ~NodeEditor();
    void onFrame();

    void setFrameBuffer(std::shared_ptr<FrameBuffer> const& framebuffer) {
        m_framebuffer = framebuffer;
    }

    void setRenderer(std::shared_ptr<Renderer> const& renderer) {
        m_renderer = renderer;
    }

    ImU32 output() const { return m_output; }
private:
    struct UiNode {
        int id;
        NodeType type;
        union {
            struct {
                int lhs, rhs;
            } add;
            struct {
                int input;
            } output;
            struct {
                int input;
            } sin;
            struct {
                int input;
            } debug;
            struct {
                int time, scale;

            } noise_shader;
        } ui;
    };

    int m_root_node_id;
    ImU32 m_output {};
    Graph<Node> m_graph {};
    std::vector<UiNode> m_nodes {};
    std::shared_ptr<FrameBuffer> m_framebuffer {};
    std::shared_ptr<Renderer> m_renderer {};
};


#endif // NODEEDITOR_H
