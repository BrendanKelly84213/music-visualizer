//
// Created by brendan on 10/18/24.
//

#ifndef GRAPH_H
#define GRAPH_H

#include <cassert>
#include <stack>
#include <unordered_map>
#include <vector>

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
        if (m_node_neighbours[from_id].empty()) {
            m_node_neighbours[from_id] = std::vector<int>{};
        }
        m_node_neighbours[from_id].push_back(to_id);
        return id;
    }

    NodeType& node(int id)
    {
        auto it = m_nodes.find(id);
        assert(it != m_nodes.end());
        return it->second;
    }

    std::unordered_map<int, Edge> const& edges()
    {
        return m_edges;
    }

    std::vector<int> neighbors(int id)
    {
        auto it = m_node_neighbours.find(id);
        if (it != m_node_neighbours.end()) {
            return it->second;
        }
        return {};
    }

    void dfs_traverse(int node_id, std::vector<int>& path)
    {
        std::stack<std::pair<int, bool>> stack;
        stack.emplace(node_id, false);
        while (!stack.empty()) {
            std::pair<int, bool> vertex = stack.top();
            stack.pop();
            if (!vertex.second) {
                vertex.second = true;
                path.push_back(vertex.first);
                auto const& adjacent = neighbors(vertex.first);
                for (auto const& neighbor : adjacent) {
                    stack.push({neighbor, false});
                }
            }
        }
    }
private:
    int m_current_id {0};
    std::unordered_map<int, NodeType> m_nodes {};
    std::unordered_map<int, Edge> m_edges {};
    std::unordered_map<int, std::vector<int>> m_node_neighbours {};
};

#endif // GRAPH_H
