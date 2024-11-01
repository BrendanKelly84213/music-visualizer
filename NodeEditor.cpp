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

    ImNodes::EndNodeEditor();

    ImGui::End();
}

