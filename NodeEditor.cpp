//
// Created by brendan on 5/4/24.
//

#include "NodeEditor.h"
NodeEditor::NodeEditor()
{
    ImNodes::CreateContext();
}

NodeEditor::~NodeEditor()
{
    ImNodes::DestroyContext();
}

void NodeEditor::onFrame()
{
    ImGui::Begin("node editor");
    ImNodes::BeginNodeEditor();

    if (ImGui::BeginPopup("nodes")) {
        const ImVec2 click_pos = ImGui::GetMousePosOnOpeningCurrentPopup();

        if (ImGui::MenuItem("add")) {
            Node value { .type = NodeType::Value, .value = 0.0f };
            Node operation { .type = NodeType::Add };

            UiNode node{};
            node.type.add.lhs = m_graph.insert_node(value);
            node.type.add.rhs = m_graph.insert_node(value);
            node.id = m_graph.insert_node(operation);

            m_graph.insert_edge(node.id, node.type.add.lhs);
            m_graph.insert_edge(node.id, node.type.add.lhs);

            m_nodes.push_back(node);
//            ImNodes::SetNodeScreenSpacePos(node.id, click_pos);
        }
        ImGui::EndPopup();
    }




    if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
        ImGui::OpenPopup("nodes");
    }

    ImNodes::EndNodeEditor();

    ImGui::End();
}

