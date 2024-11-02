//
// Created by brendan on 5/4/24.
//

#ifndef NODEEDITOR_H
#define NODEEDITOR_H

#include <vector>
#include "imnodes.h"
#include "Graph.h"

enum NodeType {
    Value,
    Time,
    Add,
    Sin,
    Output,
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
private:
    struct UiNode {
        int id;
        NodeType type;
        union {
            struct {
                int lhs, rhs;
            } add;
            struct {
                int output;
            } time;
            struct {
                int input;
            } output;
        } ui;
    };

    Graph<Node> m_graph;
    std::vector<UiNode> m_nodes;
};


#endif // NODEEDITOR_H
