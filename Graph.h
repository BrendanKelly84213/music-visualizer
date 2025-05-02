//
// Created by brendan on 10/18/24.
//

#ifndef GRAPH_H
#define GRAPH_H

#include <algorithm>
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
        m_edges_from_node[from_id] += 1;
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

    // TODO: Implement
    void erase_edge(const int id)
    {
        auto it = m_edges.find(id);
        assert(it != m_edges.end());
        const Edge& edge = it->second;
        m_edges.erase(it);

        // update neighbor count
        auto from_it = m_edges_from_node.find(edge.from);
        assert(from_it != m_edges_from_node.end());
        from_it->second -= 1;

        // update neighbor list
        auto neighbors_it = m_node_neighbours.find(edge.from);
        assert(neighbors_it != m_node_neighbours.end());
        auto& neighbors = neighbors_it->second;
        neighbors.erase(std::remove(neighbors.begin(), neighbors.end(), edge.to), neighbors.end());
    }

    // TODO: Implement
    void erase_node(const int id)
    {
        auto it = m_nodes.find(id);
        assert(it != m_nodes.end());
        m_nodes.erase(it);

        // remove all edges from this node
        auto neighbors_it = m_node_neighbours.find(id);
        if (neighbors_it != m_node_neighbours.end()) {
            for (int neighbor : neighbors_it->second) {
                erase_edge(neighbor);
            }
            m_node_neighbours.erase(neighbors_it);
        }

        // remove the node from the edges
        auto edges_it = m_edges_from_node.find(id);
        if (edges_it != m_edges_from_node.end()) {
            m_edges_from_node.erase(edges_it);
        }
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
    std::unordered_map<int, int> m_edges_from_node {};
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
