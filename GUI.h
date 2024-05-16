//
// Created by brendan on 3/21/24.
//

#ifndef GUI_H
#define GUI_H


#include <memory>

#include "Music.h"
#include "Window.h"
#include "Renderer.h"
#include "imgui.h"

class Node {
public:
    Node()
    : m_name("node"), m_shouldBeDrawn(false)
    {

    }

    Node(const std::string& name)
    : m_name(name), m_shouldBeDrawn(false)
    {
    }

    virtual void draw()
    {
    }

    [[nodiscard]] bool shouldBeDrawn() const { return m_shouldBeDrawn; }
    void setShouldBeDrawn(bool shouldBeDrawn) { m_shouldBeDrawn = shouldBeDrawn; }
protected:
    bool m_shouldBeDrawn;
    std::string m_name;
};

class Time : public Node {
public:
    Time()
    : Node("time")
    {
    }

    void draw() override
    {
        ImGui::Begin(m_name.c_str());
        ImGui::Text("%f", glfwGetTime());
        ImGui::End();
    }
};

class WAV : public Node {
public:
    static std::shared_ptr<WAV> create()
    {
        auto instance = std::make_shared<WAV>();
        instance->m_music = TRY(Music::create(), nullptr);
        return instance;
    }

    WAV()
    : Node("wav")
    {
    }

    void draw() override;
private:
    std::shared_ptr<Music> m_music;
};

struct Uniform {
    std::string type;
    std::string name;
    std::string inputMethod;
    std::string function;
    std::variant<float> value;
};

class ShaderEditor {
public:
    void draw(const std::shared_ptr<Renderer>& renderer, const std::shared_ptr<Music>& music);
private:
    struct UniformModal {
        char uniformType[128] = "float";
        char uniformName[128] = "u_time";
        char uniformInputMethod[128] = "manual";

        std::shared_ptr<Uniform> draw()
        {
            ImGui::InputTextWithHint("Type", "example: for uniform float u_time -> float", uniformType, IM_ARRAYSIZE(uniformType));
            ImGui::InputTextWithHint("Name", "example: for uniform float u_time -> u_time", uniformName, IM_ARRAYSIZE(uniformName));
            ImGui::InputTextWithHint("Input Method", "example: For manual input: manual, for function: function", uniformInputMethod, IM_ARRAYSIZE(uniformInputMethod));
            if (ImGui::Button("Create")) {
                ImGui::CloseCurrentPopup();
                return std::make_shared<Uniform>(Uniform{uniformType, uniformName, uniformInputMethod, "none",  0.0f});
            }
            if (ImGui::Button("Close")) {
                ImGui::CloseCurrentPopup();
            }
            return nullptr;
        }
    };

    std::vector<Uniform> m_uniforms;
    bool m_shouldRenderShader {false};
    std::string m_compileMessage;
    std::string m_vertexShaderPath;
    std::string m_fragmentShaderPath;
    std::shared_ptr<Shader> m_shader;
    UniformModal m_uniformModal;
};

class GUI {
public:
    ~GUI();
    void init(const Window& window);
    void mainMenu();
    void debug();
    static void render();
    void newFrame();
    void onFrame();
private:
    void addNode(const std::string& name);

    std::string m_currentShaderQuad;
    // FIXME: We probably want to be able to render more than one shader at a time
    ShaderEditor m_shaderEditor;
    std::unordered_map<std::string, std::shared_ptr<Node>> m_nodes;
};

#endif //GUI_H
