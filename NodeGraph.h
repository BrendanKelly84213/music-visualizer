//
// Created by brendan on 5/4/24.
//

#ifndef NODEGRAPH_H
#define NODEGRAPH_H

#include <vector>
#include "imnodes.h"

struct Attribute {
    int id;
    const char* name;
};

struct Node {
    int id;
    const char* name;
    std::vector<Attribute> inAttributes{};
    std::vector<Attribute> staticAttributes{};
    std::vector<Attribute> outAttributes{};
};

struct Link {
    int id;
    Attribute startAttribute;
    Attribute endAttribute;
};

class NodeGraph {
public:
    NodeGraph();
    ~NodeGraph();
    void onFrame();
private:
    std::vector<Node> m_nodes;
    std::vector<Link> m_links;
    int m_currentId;
};


#endif //NODEGRAPH_H
