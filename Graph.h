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

    int insert_node(NodeType const& node)
    {
        int id = m_current_id++;
        m_nodes.insert(std::make_pair(id, node));
        return id;
    }

    int insert_edge(int from_id, int to_id)
    {
        int id = m_current_id++;
        m_edges.insert(std::make_pair(id, Edge{.id = id, .from = from_id, .to = to_id}));
        return id;
    }

private:
    int m_current_id {0};

    std::unordered_map<int, NodeType> m_nodes;
    std::unordered_map<int, Edge> m_edges;
};


#endif // GRAPH_H
