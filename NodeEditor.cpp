//
// Created by brendan on 5/4/24.
//

#include "NodeEditor.h"
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>
NodeEditor::NodeEditor() : m_root_node_id(-1)
{
    ImNodes::CreateContext();
}

NodeEditor::~NodeEditor()
{
    ImNodes::DestroyContext();
}

static std::string node_type_enum_to_string(Node const& node)
{
    switch (node.type) {
    case NodeType::Value:
        return "Value";
    case Time:
        return "Time";
    case Add:
        return "Add";
    case Sin:
        return "Sin";
    case Output:
        return "Output";
    default: break;
    }
    return "Unknown";
}


static ImU32 evaluate(const Graph<Node>& graph, int root)
{
    std::stack<int> postorder;
    dfs_traverse(graph, root, [&postorder](int node_id) {
        postorder.push(node_id);
    });

    std::stack<float> value_stack;
    // INputs (values) always come before operation nodes.
    // For example to add two numbers together with the addition node the stack will always be
    //  value
    //  value
    //  addition
    // So when we get to an addition node, we will always have the relevant values on the stack
    while (!postorder.empty()) {
        const int id = postorder.top();
        postorder.pop();
        Node const& node = graph.node(id);
        ImGui::Text("node id: %d, node type: %s", id, node_type_enum_to_string(node).c_str());

        switch (node.type) {
        case NodeType::Add: {
            const float rhs = value_stack.top();
            value_stack.pop();
            const float lhs = value_stack.top();
            value_stack.pop();
            value_stack.push(lhs + rhs);
        }
        break;
        case NodeType::Sin: {
            const float x = value_stack.top();
            value_stack.pop();
            const float res = std::abs(std::sin(x));
            value_stack.push(res);
        }
        break;
        case NodeType::Time: {
            value_stack.push(static_cast<float>(glfwGetTime()));
        }
        break;
        case NodeType::Value: {
            // If the edge does not have an edge connecting to another node, then just use the value
            // at this node. It means the node's input pin has not been connected to anything and
            // the value comes from the node's UI.
            if (graph.num_edges_from_node(id) == 0ull) {
                value_stack.push(node.value);
            }
        }
        break;
        default: break;
        }
    }

    ImGui::Text("value stack size: %zu", value_stack.size());
    while (!value_stack.empty()) {
        ImGui::Text("value: %f", value_stack.top());
        value_stack.pop();
    }
    return 0;
}

void NodeEditor::onFrame()
{
    ImGui::Begin("node editor");
    ImNodes::BeginNodeEditor();

    if (!ImGui::IsAnyItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
        ImGui::OpenPopup("nodes");
    }

    if (ImGui::BeginPopup("nodes")) {
        ImVec2 const click_pos = ImGui::GetMousePosOnOpeningCurrentPopup();

        if (ImGui::MenuItem("add")) {
            Node value { .type = NodeType::Value, .value = 0.0f };
            Node operation { .type = NodeType::Add };

            UiNode node {};
            node.type = NodeType::Add;
            node.ui.add.lhs = m_graph.insert_node(value);
            node.ui.add.rhs = m_graph.insert_node(value);
            node.id = m_graph.insert_node(operation);

            m_graph.insert_edge(node.id, node.ui.add.lhs);
            m_graph.insert_edge(node.id, node.ui.add.lhs);

            m_nodes.push_back(node);
            ImNodes::SetNodeScreenSpacePos(node.id, click_pos);
        }

        if (ImGui::MenuItem("output")) {
            Node value {.type = NodeType::Value, .value = 0.0f };
            Node operation {.type = NodeType::Output };

            UiNode node {};
            node.type = NodeType::Output;
            node.ui.output.input = m_graph.insert_node(value);
            node.id = m_graph.insert_node(operation);

            m_graph.insert_edge(node.id, node.ui.output.input);

            m_nodes.push_back(node);
            ImNodes::SetNodeScreenSpacePos(node.id, click_pos);
            m_root_node_id = node.id;
        }

        if (ImGui::MenuItem("sin")) {
            Node value {.type = NodeType::Value, .value = 0.0f };
            Node operation {.type = NodeType::Sin };

            UiNode node {};
            node.type = NodeType::Sin;
            // We don't need an output node because the value will be computed during the evaluation step
            // and pushed onto the value stack
            node.ui.sin.input = m_graph.insert_node(value);
            node.id = m_graph.insert_node(operation);

            m_graph.insert_edge(node.id, node.ui.sin.input);

            m_nodes.push_back(node);
            ImNodes::SetNodeScreenSpacePos(node.id, click_pos);
        }
        if (ImGui::MenuItem("time")) {
            Node operation {.type = NodeType::Time };
            UiNode node {};
            node.type = NodeType::Time;
            node.id = m_graph.insert_node(operation);
            m_nodes.push_back(node);
            ImNodes::SetNodeScreenSpacePos(node.id, click_pos);
        }
        ImGui::EndPopup();
    }

    for (auto const& node : m_nodes) {
        switch (node.type) {
        case Add:
            ImNodes::BeginNode(node.id);
            ImNodes::BeginNodeTitleBar();
            ImGui::TextUnformatted("add");
            ImNodes::EndNodeTitleBar();

            ImNodes::BeginInputAttribute(node.ui.add.lhs);
            ImGui::TextUnformatted("lhs");
            ImNodes::EndInputAttribute();

            ImNodes::BeginInputAttribute(node.ui.add.rhs);
            ImGui::TextUnformatted("rhs");
            ImGui::SameLine();
            ImNodes::EndInputAttribute();

            ImNodes::BeginOutputAttribute(node.id);
            ImGui::TextUnformatted("result");
            ImNodes::EndOutputAttribute();
            ImNodes::EndNode();
            break;
        case Output:
            ImNodes::BeginNode(node.id);
            ImNodes::BeginInputAttribute(node.ui.output.input);
            ImGui::TextUnformatted("output");
            ImNodes::EndInputAttribute();
            ImNodes::EndNode();
            break;
        case Sin:
            ImNodes::BeginNode(node.id);

            ImNodes::BeginInputAttribute(node.ui.sin.input);
            ImNodes::EndInputAttribute();
            ImNodes::BeginOutputAttribute(node.id);
            ImGui::TextUnformatted("sin");
            ImNodes::EndOutputAttribute();
            ImNodes::EndNode();
            break;
        case Time:
            ImNodes::BeginNode(node.id);
            ImNodes::BeginOutputAttribute(node.id);
            ImGui::TextUnformatted("time");
            ImNodes::EndOutputAttribute();
            ImNodes::EndNode();
        default:
            break;
        }
    }


    // Create links
    for (auto const& edge_pair : m_graph.edges()) {
        auto const& edge = edge_pair.second;
        // We don't want to link internal nodes
        if (m_graph.node(edge.from).type != NodeType::Value) {
            continue;
        }

        ImNodes::Link(edge.id, edge.from, edge.to);
    }

    ImNodes::EndNodeEditor();

    int start_attribute, end_attribute;
    if (ImNodes::IsLinkCreated(&start_attribute, &end_attribute)) {
        NodeType start_type = m_graph.node(start_attribute).type;
        NodeType end_type = m_graph.node(end_attribute).type;

        if (start_type != end_type) {
            if (start_type != NodeType::Value) {
                std::swap(start_attribute, end_attribute);
            }

            m_graph.insert_edge(start_attribute, end_attribute);
        }
    }

    ImGui::End();

    // Evaluate the graph
    m_output = m_root_node_id != -1 ? evaluate(m_graph, m_root_node_id) : 0;
}

