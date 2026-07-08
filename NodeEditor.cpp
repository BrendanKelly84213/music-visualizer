//
// Created by brendan on 5/4/24.
//

#include "NodeEditor.h"
#include "FrameBuffer.h"
#include "RenderCommand.h"
#include "Shader.h"
#include <GLFW/glfw3.h>
#include <algorithm>
#include <cmath>
#include <iostream>
NodeEditor::NodeEditor()
    : m_root_node_id(-1),
    m_framebuffer(nullptr)
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
    case Debug:
        return "Debug";
    case NodeType::StaticValue:
        return "StaticValue";
    case NodeType::NoiseShader:
        return "NoiseShader";
    default: break;
    }
    return "Unknown";
}

struct RenderState {
    std::shared_ptr<Shader> current_shader;
    std::shared_ptr<FrameBuffer> framebuffer;
    std::shared_ptr<Renderer> renderer;
    // Add rendering-related state as needed
};

static ImU32 evaluate(const Graph<Node>& graph, int root, RenderState& state)
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
            const float res = std::sin(x);
            value_stack.push(res);
        }
        break;
        case NodeType::Time: {
            value_stack.push(static_cast<float>(glfwGetTime()));
        }
        break;
        // I still don't get this
        case NodeType::Value: {
            // If the edge does not have an edge connecting to another node, then just use the value
            // at this node. It means the node's input pin has not been connected to anything and
            // the value comes from the node's UI.
            if (graph.num_edges_from_node(id) == 0ull) {
                value_stack.push(node.value);
            }
        }
        break;
        case NodeType::StaticValue: {
            value_stack.push(node.value);
        }
        break;
        case NodeType::NoiseShader: {
            float u_scale = value_stack.top();
            value_stack.pop();
            float u_time = value_stack.top();
            value_stack.pop();
            ImGui::Text("u_time: %f, u_scale: %f", u_time, u_scale);

            // FIXME: it's time to make this dynamic. Finally hit the wall with this hardcodeded business
            auto err = state.renderer->loadShader("noise-shader", "/home/brendan/dev/my-stuff/music-visualizer/assets/shaders/vertex-shader.glsl", "/home/brendan/dev/my-stuff/music-visualizer/assets/shaders/noise-fragment-shader.glsl");
            if (err.isError()) {
                ImGui::Text("error loading shader: %s", err.error().message().c_str());
                return 0;
            }

            state.current_shader = err.value();
            if (!state.current_shader->loaded()) {
                ImGui::Text("error loading shader");
                return 0;
            }

            // FIXME: rework this API
            state.current_shader->use();
            state.current_shader->setUniform1f("u_time", u_time);
            state.current_shader->setUniform1f("u_scale", u_scale);
            state.current_shader->setUniform2f("u_resolution", float(state.framebuffer->width()), float(state.framebuffer->height()));
            state.current_shader->setUniformMat4f("u_transform", glm::mat4(1.0f));

            RenderCommand::setClearColor({0.0,0.0,0.1, 1.0});
            RenderCommand::clear();

            state.renderer->vertexBuffer()->setLayout({
                {VertexLayoutType::Float3, "aPos"}
            });

            state.renderer->vertexArray()->addVertexBuffer(state.renderer->vertexBuffer());
        }
        break;
        case NodeType::FunnyLiquidShader: {
            float u_time = value_stack.top();
            value_stack.pop();
            // Properties
            float u_lacunarity = value_stack.top();
            value_stack.pop();
            float u_octaves = value_stack.top();
            value_stack.pop();
            float u_gain = value_stack.top();
            // Initial values
            float u_initialAmplitude = value_stack.top();
            value_stack.pop();
            float u_initialFrequency = value_stack.top();
            value_stack.pop();
            float u_speed = value_stack.top();
            value_stack.pop();
            float u_something = value_stack.top();
            value_stack.pop();

            auto err = state.renderer->loadShader("noise-shader", "/home/brendan/dev/my-stuff/music-visualizer/assets/shaders/vertex-shader.glsl", "/home/brendan/dev/my-stuff/music-visualizer/assets/shaders/fbm-funny-liquid.glsl");
            if (err.isError()) {
                ImGui::Text("error loading shader: %s", err.error().message().c_str());
                return 0;
            }

            state.current_shader = err.value();
            if (!state.current_shader->loaded()) {
                ImGui::Text("error loading shader");
                return 0;
            }

            // FIXME: rework this API
            state.current_shader->use();
            state.current_shader->setUniform1f("u_time", u_time);
            state.current_shader->setUniform1f("u_lacunarity", u_lacunarity);
            state.current_shader->setUniform1f("u_octaves", u_octaves);
            state.current_shader->setUniform1f("u_gain", u_gain);
            state.current_shader->setUniform1f("u_initialAmplitude", u_initialAmplitude);
            state.current_shader->setUniform1f("u_initialFrequency", u_initialFrequency);
            state.current_shader->setUniform1f("u_speed", u_speed);
            state.current_shader->setUniform1f("u_something", u_something);
            state.current_shader->setUniform2f("u_resolution", float(state.framebuffer->width()), float(state.framebuffer->height()));
            state.current_shader->setUniformMat4f("u_transform", glm::mat4(1.0f));

            RenderCommand::setClearColor({0.0,0.0,0.1, 1.0});
            RenderCommand::clear();

            state.renderer->vertexBuffer()->setLayout({
                {VertexLayoutType::Float3, "aPos"}
            });

            // What the fuck is this bro
            state.renderer->vertexArray()->addVertexBuffer(state.renderer->vertexBuffer());
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

    // Output is always the root node so we don't bother putting it in the switch statement
    ImGui::Begin("output window");
    {
        if (state.framebuffer == nullptr) {
            ImGui::Text("framebuffer is null");
            return 0;
        }
        auto size = ImGui::GetContentRegionAvail();
        auto pos = ImGui::GetCursorScreenPos();
        unsigned int colorBufferId = state.framebuffer->textureColorBuffer();
        state.framebuffer->rescale((int)size.x, (int)size.y);
        ImGui::GetWindowDrawList()->AddImage((void*)colorBufferId, pos, {pos.x + size.x, pos.y + size.y}, {0, 1}, {1,0});

        if (state.framebuffer == nullptr) {
            ImGui::Text("framebuffer is null");
            return 0;
        }
        state.framebuffer->bind();
        // FIXME: Make more flexible? I don't know how this is gonna look in the future tbh
        // But it'd be nice to be able to draw more than one shader or whatever...
        RenderCommand::drawIndexed(6);
        state.framebuffer->unbind();
        ImGui::End();
    }

    return 0;
}

void NodeEditor::onFrame()
{
    static RenderState state {};

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

        if (ImGui::MenuItem("debug")) {
            Node value {.type = NodeType::Value, .value = 0.0f };
            Node operation {.type = NodeType::Debug };

            UiNode node {};
            node.type = NodeType::Debug;
            node.ui.debug.input = m_graph.insert_node(value);
            node.id = m_graph.insert_node(operation);

            m_graph.insert_edge(node.id, node.ui.debug.input);

            m_nodes.push_back(node);
            ImNodes::SetNodeScreenSpacePos(node.id, click_pos);
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

        if (ImGui::MenuItem("noise-shader")) {
            if (m_framebuffer != nullptr) {
                state.framebuffer = m_framebuffer;
            }
            if (m_renderer != nullptr) {
                state.renderer = m_renderer;
            }

            Node value {.type = NodeType::Value, .value = 0.0f };
            Node operation {.type = NodeType::NoiseShader };

            UiNode node {};
            node.type = NodeType::NoiseShader;
            node.ui.noise_shader.time = m_graph.insert_node(value);
            node.ui.noise_shader.scale = m_graph.insert_node(value);

            node.id = m_graph.insert_node(operation);

            m_graph.insert_edge(node.id, node.ui.noise_shader.time);
            m_graph.insert_edge(node.id, node.ui.noise_shader.scale);

            m_nodes.push_back(node);
            ImNodes::SetNodeScreenSpacePos(node.id, click_pos);
        }

        if (ImGui::MenuItem("funny-liquid")) {
            if (m_framebuffer != nullptr) {
                state.framebuffer = m_framebuffer;
            }
            if (m_renderer != nullptr) {
                state.renderer = m_renderer;
            }

            Node operation {.type = NodeType::FunnyLiquidShader };

            UiNode node {};
            node.type = NodeType::FunnyLiquidShader;

            // Create input nodes for each uniform
            node.ui.funny_liquid_shader.time = m_graph.insert_node({.type = NodeType::Value, .value = 0.0f});
            node.ui.funny_liquid_shader.lacunarity = m_graph.insert_node({.type = NodeType::Value, .value = 2.0f});
            node.ui.funny_liquid_shader.octaves = m_graph.insert_node({.type = NodeType::Value, .value = 4.0f});
            node.ui.funny_liquid_shader.gain = m_graph.insert_node({.type = NodeType::Value, .value = 0.5f});
            node.ui.funny_liquid_shader.initial_amplitude = m_graph.insert_node({.type = NodeType::Value, .value = 1.0f});
            node.ui.funny_liquid_shader.initial_frequency = m_graph.insert_node({.type = NodeType::Value, .value = 1.0f});
            node.ui.funny_liquid_shader.speed = m_graph.insert_node({.type = NodeType::Value, .value = 1.0f});
            node.ui.funny_liquid_shader.something = m_graph.insert_node({.type = NodeType::Value, .value = 1.0f});

            node.id = m_graph.insert_node(operation);

            // Connect inputs to the FunnyLiquidShader node
            m_graph.insert_edge(node.id, node.ui.funny_liquid_shader.time);
            m_graph.insert_edge(node.id, node.ui.funny_liquid_shader.lacunarity);
            m_graph.insert_edge(node.id, node.ui.funny_liquid_shader.octaves);
            m_graph.insert_edge(node.id, node.ui.funny_liquid_shader.gain);
            m_graph.insert_edge(node.id, node.ui.funny_liquid_shader.initial_amplitude);
            m_graph.insert_edge(node.id, node.ui.funny_liquid_shader.initial_frequency);
            m_graph.insert_edge(node.id, node.ui.funny_liquid_shader.speed);
            m_graph.insert_edge(node.id, node.ui.funny_liquid_shader.something);

            m_nodes.push_back(node);
            ImNodes::SetNodeScreenSpacePos(node.id, click_pos);
        }

        if (ImGui::MenuItem("static-value")) {
            Node operation {.type = NodeType::StaticValue };
            UiNode node {};
            node.type = NodeType::StaticValue;
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
        case Debug:
            ImNodes::BeginNode(node.id);
            ImNodes::BeginInputAttribute(node.ui.debug.input);
            ImGui::TextUnformatted("debug");
            ImNodes::EndInputAttribute();
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
            break;
        case NoiseShader:
            ImNodes::BeginNode(node.id);
            ImNodes::BeginNodeTitleBar();
            ImGui::TextUnformatted("noise-shader");
            ImNodes::EndNodeTitleBar();

            ImNodes::BeginInputAttribute(node.ui.noise_shader.time);
            ImGui::TextUnformatted("time");
            ImNodes::EndInputAttribute();
            ImNodes::BeginInputAttribute(node.ui.noise_shader.scale);
            ImGui::TextUnformatted("scale");
            ImNodes::EndInputAttribute();

            ImNodes::BeginOutputAttribute(node.id);
            ImGui::TextUnformatted("result");
            ImNodes::EndOutputAttribute();
            ImNodes::EndNode();
            break;
        case StaticValue: {
            ImNodes::BeginNode(node.id);
            ImNodes::BeginNodeTitleBar();
            ImGui::TextUnformatted("static-value");
            ImNodes::EndNodeTitleBar();

            ImNodes::BeginStaticAttribute(node.id);
            ImGui::PushItemWidth(120.0f);
            ImGui::DragFloat("value", &m_graph.node_ptr(node.id)->value, 0.01f);
            ImNodes::EndStaticAttribute();
            ImGui::PopItemWidth();

            ImNodes::BeginOutputAttribute(node.id);
            ImNodes::EndOutputAttribute();
            ImNodes::EndNode();
        }
        break;
        case FunnyLiquidShader:
            ImNodes::BeginNode(node.id);
            ImNodes::BeginNodeTitleBar();
            ImGui::TextUnformatted("funny-liquid");
            ImNodes::EndNodeTitleBar();

            ImNodes::BeginInputAttribute(node.ui.funny_liquid_shader.time);
            ImGui::TextUnformatted("time");
            ImNodes::EndInputAttribute();

            ImNodes::BeginInputAttribute(node.ui.funny_liquid_shader.lacunarity);
            ImGui::TextUnformatted("lacunarity");
            ImNodes::EndInputAttribute();

            ImNodes::BeginInputAttribute(node.ui.funny_liquid_shader.octaves);
            ImGui::TextUnformatted("octaves");
            ImNodes::EndInputAttribute();

            ImNodes::BeginInputAttribute(node.ui.funny_liquid_shader.gain);
            ImGui::TextUnformatted("gain");
            ImNodes::EndInputAttribute();

            ImNodes::BeginInputAttribute(node.ui.funny_liquid_shader.initial_amplitude);
            ImGui::TextUnformatted("initial_amplitude");
            ImNodes::EndInputAttribute();

            ImNodes::BeginInputAttribute(node.ui.funny_liquid_shader.initial_frequency);
            ImGui::TextUnformatted("initial_frequency");
            ImNodes::EndInputAttribute();

            ImNodes::BeginInputAttribute(node.ui.funny_liquid_shader.speed);
            ImGui::TextUnformatted("speed");
            ImNodes::EndInputAttribute();

            ImNodes::BeginInputAttribute(node.ui.funny_liquid_shader.something);
            ImGui::TextUnformatted("something");
            ImNodes::EndInputAttribute();

            ImNodes::BeginOutputAttribute(node.id);
            ImGui::TextUnformatted("result");
            ImNodes::EndOutputAttribute();
            ImNodes::EndNode();
            break;
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

    // Handle neew nodes
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

    // Handle deleted nodes and edgese
    {
        int link_id;
        if (ImNodes::IsLinkDestroyed(&link_id))
        {
            m_graph.erase_edge(link_id);
        }
    }

    {
        const int num_selected = ImNodes::NumSelectedLinks();
        if (num_selected > 0 && ImGui::IsKeyReleased(ImGuiKey_X))
        {
            static std::vector<int> selected_links;
            selected_links.resize(static_cast<size_t>(num_selected));
            ImNodes::GetSelectedLinks(selected_links.data());
            for (const int edge_id : selected_links)
            {
                m_graph.erase_edge(edge_id);
            }
        }
    }

    {
        const int num_selected = ImNodes::NumSelectedNodes();
        if (num_selected > 0 && ImGui::IsKeyReleased(ImGuiKey_X))
        {
            static std::vector<int> selected_nodes;
            selected_nodes.resize(static_cast<size_t>(num_selected));
            ImNodes::GetSelectedNodes(selected_nodes.data());
            for (const int node_id : selected_nodes)
            {
                m_graph.erase_node(node_id);
                auto iter = std::find_if(
                    m_nodes.begin(), m_nodes.end(), [node_id](const UiNode& node) -> bool {
                        return node.id == node_id;
                    });
                // Erase any additional internal nodes
                switch (iter->type)
                {
                case NodeType::Add:
                    m_graph.erase_node(iter->ui.add.lhs);
                    m_graph.erase_node(iter->ui.add.rhs);
                    break;
                case NodeType::Output:
                    m_graph.erase_node(iter->ui.output.input);
                    m_root_node_id = -1;
                    break;
                case NodeType::Sin:
                    m_graph.erase_node(iter->ui.sin.input);
                    break;
                default:
                    break;
                }
                m_nodes.erase(iter);
            }
        }
    }

    ImGui::End();

    // Evaluate the graph
    m_output = m_root_node_id != -1 ? evaluate(m_graph, m_root_node_id, state) : 0;
}

