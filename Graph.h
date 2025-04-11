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

    NodeType* node_ptr(int id)
    {
        auto it = m_nodes.find(id);
        assert(it != m_nodes.end());
        return &it->second;
    }

    const NodeType& node(int id) const
    {
        auto it = m_nodes.find(id);
        assert(it != m_nodes.end());
        return it->second;
    }

    std::unordered_map<int, Edge> const& edges()
    {
        return m_edges;
    }

    std::vector<int> neighbors(int id) const
    {
        auto it = m_node_neighbours.find(id);
        if (it != m_node_neighbours.end()) {
            return it->second;
        }
        return {};
    }

    int num_edges_from_node(int id) const
    {
        auto it = m_node_neighbours.find(id);
        if (it != m_node_neighbours.end()) {
            return static_cast<int>(it->second.size());
        }
        return 0;
    }

private:
    int m_current_id {0};
    std::unordered_map<int, NodeType> m_nodes {};
    std::unordered_map<int, Edge> m_edges {};
    std::unordered_map<int, std::vector<int>> m_node_neighbours {};
};

template<typename NodeType, typename Visitor>
void dfs_traverse(const Graph<NodeType> graph, const int start_node, Visitor visitor)
{
    std::stack<int> stack;

    stack.push(start_node);

    while (!stack.empty())
    {
        const int current_node = stack.top();
        stack.pop();

        visitor(current_node);

        for (const int neighbor : graph.neighbors(current_node))
        {
            stack.push(neighbor);
        }
    }
}

#endif // GRAPH_H
