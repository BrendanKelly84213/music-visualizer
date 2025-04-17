//
// Created by brendan on 5/4/24.
//

#include "NodeEditor.h"
#include "FrameBuffer.h"
#include "RenderCommand.h"
#include "Shader.h"
#include <GLFW/glfw3.h>
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

// This function traverses the graph in a depth-first manner and pushes the node ids onto the stack
// The function evaluates nodes as numerical values and is called when the root node is set (currently when the user creates an 'output' node)
// In the future we might want different types of output nodes, graphical, sound, etc... from one graph
// The key thing to understand is that the evaluate function could be called from any root.
// Currently this "evaluate" function acts as a debug function to show the values of the nodes
// If we wanted to have a graphical output node, the evaluation function would evaluate the graph treating that node as root.
// Creating that node would spawn a little output window and scale a framebuffer. Whatever node is immediately connected to the output node
// Would be what is drawn to the window. In touch designer I think it just figures out what the best way to do draw what is connected...
// We would have to come up with our own rules about what to draw, how to best graphically represent "sin" for example
// So the output node should have some kind of internal logic about what is drawn and how to draw it.
// If it is at all possible to abstract values into something that can be drawn to the point where it can
// Be any scene, this would be ideal. That way the output node would not need any knowledge about what it is connected to
//

// COPILOT:
/*
 * Yes, what you're trying to do is possible, and there are abstractions you can use to simplify the process. The key is to design a system that decouples the evaluation of the node graph from the rendering logic. Here's how you can approach it:

### 1. **Abstract Node Evaluation**
   - Each node in your graph should have a well-defined interface for evaluation. For example, a `Node` could have a method like `evaluate()` that computes its value based on its inputs.
   - The `evaluate()` method can return a generic data structure (e.g., a `Variant` type or a union) that represents the result, whether it's a numerical value, a texture, or a shader program.

### 2. **Output Node as a Renderer**
   - Treat the output node as a renderer. Instead of directly evaluating the graph for a numerical value, the output node could evaluate its inputs and use the result to configure a rendering pipeline.
   - For example, if the output node is connected to a "Sin" node, it could use the evaluated value to modify a shader uniform or generate a texture.

### 3. **Graphical Representation**
   - Use a common abstraction for rendering, such as a `RenderTarget` or `FrameBuffer`, to represent the output of the graph. Each node could contribute to this target by drawing its result or modifying the state of the rendering pipeline.
   - For example, a "Sin" node could generate a texture that represents a sine wave, and the output node could composite this texture onto the screen.

### 4. **Shader Support**
   - Nodes that require shader support (e.g., procedural textures or effects) can encapsulate their logic in a shader program. The node's `evaluate()` method could compile and return a shader or set up the necessary uniforms for rendering.

### 5. **Dynamic Rendering Rules**
   - To avoid hardcoding logic for each type of node, you can use a system of "rendering rules" or "node handlers." Each node type could register a handler that knows how to evaluate and render that node.
   - For example, a "Sin" node handler could generate a texture, while an "Add" node handler could combine two textures.

### 6. **Example Workflow**
   - Traverse the graph to evaluate all nodes, starting from the root (output node).
   - Each node contributes its result to a `RenderTarget` or modifies the rendering pipeline.
   - The output node takes the final result and draws it to the screen.

### 7. **Tools and Libraries**
   - Use libraries like ImGui and ImNodes for the UI and graph editing.
   - Use OpenGL or Vulkan for rendering, with abstractions like `Shader`, `FrameBuffer`, and `RenderCommand` to manage the pipeline.

By designing your system with these abstractions, you can avoid hardcoding complex logic for each node type and make your graph extensible and flexible for different types of outputs.

 Yes, it is possible to implement the graphical pipeline using your current DFS traversal logic with a "visitor" callback. However, as you mentioned, the value stack method is more suited for numerical evaluation and would need to be adapted for graphical rendering. Instead of pushing numerical values onto a stack, you would propagate rendering commands or state changes through the graph.

Here’s how you might approach it:

   ### Key Changes
   1. **State Propagation**: Replace the value stack with a rendering state object that gets modified as you traverse the graph. This state could include shader programs, textures, transformation matrices, etc.
       2. **Node Handlers**: Each node type should have a handler that knows how to modify the rendering state or issue rendering commands.
       3. **Output Node**: The output node would finalize the rendering by drawing to the screen or a framebuffer.

       ### Pseudo Code Example
           Here’s a high-level pseudo code example:

```cpp
   struct RenderState {
   Shader* currentShader;
   Texture* currentTexture;
   glm::mat4 transform;
   // Add other rendering-related state as needed
};

void evaluateGraphForRendering(const Graph<Node>& graph, int root, RenderState& state) {
   std::stack<int> postorder;
   dfs_traverse(graph, root, [&postorder](int node_id) {
       postorder.push(node_id);
   });

   while (!postorder.empty()) {
       int node_id = postorder.top();
       postorder.pop();
       const Node& node = graph.node(node_id);

       switch (node.type) {
       case NodeType::Value:
           // Set a uniform or texture based on the value
           state.currentShader->setUniform1f("u_value", node.value);
           break;

       case NodeType::Sin:
           // Modify the state to apply a sine wave effect
           state.currentShader->setUniform1f("u_sinEffect", std::sin(node.value));
           break;

       case NodeType::Add:
           // Combine two textures or values
           combineTextures(state.currentTexture, node.input1, node.input2);
           break;

       case NodeType::Output:
           // Finalize rendering
           drawToScreen(state);
           break;

       default:
           break;
       }
   }
}

void drawToScreen(const RenderState& state) {
   // Bind the shader, set uniforms, and draw the final output
   state.currentShader->use();
   state.currentTexture->bind();
   RenderCommand::drawIndexed(6);
}
```

   ### Explanation
   1. **RenderState**: This struct holds the current rendering state, such as the active shader, texture, and transformation matrix.
         2. **DFS Traversal**: The graph is traversed in postorder, ensuring that inputs are processed before their dependent nodes.
       3. **Node Handlers**: Each node type modifies the `RenderState` or issues rendering commands. For example, a `Sin` node might set a shader uniform, while an `Add` node might combine two textures.
       4. **Output Node**: The output node uses the final state to render the result to the screen or a framebuffer.

       ### Benefits
       - **Flexibility**: The rendering logic is decoupled from the graph structure, making it easier to add new node types.
   - **Extensibility**: You can extend the `RenderState` and node handlers to support more complex rendering features, such as procedural textures or post-processing effects.

                                           This approach should integrate well with your existing graph logic and allow you to implement a flexible graphical pipeline.
 */

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

            auto err = state.renderer->loadShader("noise-shader", "/home/brendan/dev/my-stuff/music-visualizer/assets/shaders/vertex-shader.glsl", "/home/brendan/dev/my-stuff/music-visualizer/assets/shaders/noise-fragment-shader.glsl");
            if (err.isError()) {
                ImGui::Text("error loading shader: %s", err.error().message().c_str());
                ImGui::End();
                return 0;
            }

            state.current_shader = err.value();
            if (!state.current_shader->loaded()) {
                ImGui::Text("error loading shader");
                ImGui::End();
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

            // What the fuck is this bro
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
                ImGui::End();
                return 0;
            }

            state.current_shader = err.value();
            if (!state.current_shader->loaded()) {
                ImGui::Text("error loading shader");
                ImGui::End();
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
            ImGui::End();
            return 0;
        }
        auto size = ImGui::GetContentRegionAvail();
        auto pos = ImGui::GetCursorScreenPos();
        unsigned int colorBufferId = state.framebuffer->textureColorBuffer();
        state.framebuffer->rescale((int)size.x, (int)size.y);
        ImGui::GetWindowDrawList()->AddImage((void*)colorBufferId, pos, {pos.x + size.x, pos.y + size.y}, {0, 1}, {1,0});

        if (state.framebuffer == nullptr) {
            ImGui::Text("framebuffer is null");
            ImGui::End();
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
    m_output = m_root_node_id != -1 ? evaluate(m_graph, m_root_node_id, state) : 0;
}

