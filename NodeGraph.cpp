//
// Created by brendan on 5/4/24.
//

#include "NodeGraph.h"
NodeGraph::NodeGraph()
{
    ImNodes::CreateContext();
}

NodeGraph::~NodeGraph()
{
    ImNodes::DestroyContext();
}

// Could be put into a config file
Node NodeLibrary[] = {
{
        .name = "Time",
        .inAttributes = {

        },
        .staticAttributes = {

        },
        .outAttributes = {
            {.name = "out"}
        }
    },
{
        .name = "Add",
        .inAttributes = {
        {.name = "in1"},
        {.name = "in2"}
        },
        .staticAttributes = {
        },
        .outAttributes = {
            {.name = "out"},
        }
    },

};

void NodeGraph::onFrame()
{
    ImGui::Begin("node editor");
    ImNodes::BeginNodeEditor();

    if (ImGui::BeginPopup("node selector")) {
        for (auto const& node : NodeLibrary) {
            auto nodeName = node.name;
            if (ImGui::Selectable(nodeName)) {
                m_nodes.push_back(node);
                auto& nodeToBeEdited = m_nodes.back();
                nodeToBeEdited.id = m_currentId++;
                size_t i = 1;
                for (auto& attribute : nodeToBeEdited.inAttributes) {
                    attribute.id = nodeToBeEdited.id << 8 * i;
                    ++i;
                }
                for (auto& attribute : nodeToBeEdited.staticAttributes) {
                    attribute.id = nodeToBeEdited.id << 8 * i;
                    ++i;
                }
                for (auto& attribute : nodeToBeEdited.outAttributes) {
                    attribute.id = nodeToBeEdited.id << 8 * i;
                    ++i;
                }
            }
        }
        ImGui::EndPopup();
    }

    if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
        ImGui::OpenPopup("node selector");
    }

    for (auto const& node : m_nodes) {
        ImNodes::BeginNode(node.id);
        ImNodes::BeginNodeTitleBar();
        ImGui::TextUnformatted(node.name);
        ImNodes::EndNodeTitleBar();

        for (auto const& inAttribute : node.inAttributes) {
            ImNodes::BeginInputAttribute(inAttribute.id);
            ImGui::TextUnformatted(inAttribute.name);
            ImNodes::EndInputAttribute();
        }

        for (auto const& staticAttribute : node.staticAttributes) {
            ImNodes::BeginStaticAttribute(staticAttribute.id);
            ImGui::TextUnformatted(staticAttribute.name);
            ImNodes::EndStaticAttribute();
        }

        for (auto const& outAttribute : node.outAttributes) {
            ImNodes::BeginOutputAttribute(outAttribute.id);
            ImGui::TextUnformatted(outAttribute.name);
            ImNodes::EndOutputAttribute();
        }

        ImNodes::EndNode();
    }

    ImNodes::EndNodeEditor();
    ImGui::End();
}

