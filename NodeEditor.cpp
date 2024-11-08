//
// Created by brendan on 5/4/24.
//

#include "NodeEditor.h"
NodeEditor::NodeEditor() : m_root_node_id(-1)
{
    ImNodes::CreateContext();
}

NodeEditor::~NodeEditor()
{
    ImNodes::DestroyContext();
}

static ImU32 evaluate(Graph<Node>& graph, int root)
{

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

