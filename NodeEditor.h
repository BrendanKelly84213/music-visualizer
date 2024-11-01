//
// Created by brendan on 5/4/24.
//

#ifndef NODEEDITOR_H
#define NODEEDITOR_H

#include <vector>
#include "imnodes.h"
#include "Graph.h"

struct Attribute {
    int id;
    const char* name;
};

enum NodeType {
    Time,
    Add,
    Sin,
    Output,
};

struct Node {
    int id;
    NodeType type;
};

class NodeEditor {
public:
    NodeEditor();
    ~NodeEditor();
    void onFrame();
private:
    Graph<Node> m_graph;
};


#endif // NODEEDITOR_H
