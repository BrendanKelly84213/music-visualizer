//
// Created by brendan on 10/18/24.
//

#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <unordered_map>

template <typename NodeType>
class Graph {
public:
    struct Edge {
        int id;
        int from;
        int to;
    };

private:
    int m_current_id;

    std::unordered_map<int, NodeType> m_nodes;
    std::unordered_map<int, Edge> m_edges;
};

#endif // GRAPH_H
