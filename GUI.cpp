//
// Created by brendan on 3/21/24.
//

#include <memory>
#include <thread>
#include <algorithm>
#include <stdexcept>
#include "GUI.h"
#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"

void ShaderEditor::draw(const std::shared_ptr<Renderer>& renderer)
{
    auto openVertexShaderFile = [&]() {
        char filename[1024];
        FILE *f = popen(R"(zenity --file-selection  --file-filter=*.glsl)", "r");
        fgets(filename, 1024, f);
        std::string filenameString = filename;
        filenameString.erase(std::remove(filenameString.begin(), filenameString.end(), '\n'), filenameString.cend());
        m_vertexShaderPath = filenameString;
    };

    auto openFragmentShaderFile = [&]() {
        char filename[1024];
        FILE *f = popen(R"(zenity --file-selection  --file-filter=*.glsl)", "r");
        fgets(filename, 1024, f);
        std::string filenameString = filename;
        filenameString.erase(std::remove(filenameString.begin(), filenameString.end(), '\n'), filenameString.cend());
        m_fragmentShaderPath = filenameString;
    };

    ImGui::Begin("Shader");
    if (ImGui::MenuItem("Open Vertex Shader")) {
        std::thread t(openVertexShaderFile);
        t.detach();
    }
    if (ImGui::MenuItem("Open Fragment Shader")) {
        std::thread t(openFragmentShaderFile);
        t.detach();
    }

    if (ImGui::Button("Compile")) {
        auto result = renderer->loadShader("custom", m_vertexShaderPath, m_fragmentShaderPath);
        if (result.isError()) {
            m_compileMessage = "Failed to compile: " + result.error().message();
            std::cout << m_compileMessage << '\n';
        } else {
            m_compileMessage = "Compiled shader";
            m_shader = result.value();
        }
    }

    if (ImGui::Selectable("Draw", m_shouldRenderShader)) {
        m_shouldRenderShader = !m_shouldRenderShader;
    }

    ImGui::Text("%s", m_compileMessage.c_str());
    if (ImGui::Button("Add Uniform")) {
        ImGui::OpenPopup("Create Uniform Modal");
    }
    if (ImGui::BeginPopupModal("Create Uniform Modal")) {
        auto uniform = m_uniformModal.draw();
        if (uniform != nullptr) {
            m_uniforms.push_back(*uniform);
        }
        ImGui::EndPopup();
    }

    ImGui::Text("number of uniforms: %zu", m_uniforms.size());

    for (size_t i = 0; i < m_uniforms.size(); ++i) {
        auto& uniform = m_uniforms[i];
        ImGui::Text("%s", std::string("uniform " + uniform.type + " " + uniform.name).c_str());
        char value[128] = "";
        memcpy(value, uniform.value.c_str(), uniform.value.size());
        // FIXME: This method of maintaining button to modal ID relation is not pretty...
        const auto editModalId = std::string("Edit Uniform Modal" + std::to_string(i));
        const auto editButtonId = std::string("Edit" + std::to_string(i));
        const auto deleteButtonId = std::string("Delete" + std::to_string(i));
        if (ImGui::Button(editButtonId.c_str())) {
            ImGui::OpenPopup(editModalId.c_str());
        }
        ImGui::SameLine();
        if (ImGui::Button(deleteButtonId.c_str())) {
            m_uniforms.erase(m_uniforms.begin() + i);
            continue;
        }
        if (ImGui::BeginPopupModal(editModalId.c_str())) {
            auto temp = m_uniformModal.draw();
            if (temp != nullptr) {
                uniform = *temp;
            }
            ImGui::EndPopup();
        }
        if (uniform.type == "float") {
            ImGui::InputText(std::string("Value " + uniform.name).c_str(), value, IM_ARRAYSIZE(value));
            uniform.value = value;
            if (m_shader == nullptr) {
                continue;
            }
            try {
                float floatValue = std::stof(uniform.value);
                m_shader->setUniform1f(uniform.name, floatValue);
            } catch (std::invalid_argument const& exception) {
                m_shader->setUniform1f(uniform.name, 0.0f);
                ImGui::Text("%s: invalid argument: %s", exception.what(), uniform.value.c_str());
            }
        } else if (uniform.type == "function") {
            ImGui::InputTextWithHint(std::string("Function Name " + uniform.name).c_str(), "glfwGetTime()", value, IM_ARRAYSIZE(value));
            if (m_shader == nullptr) {
                continue;
            }
            if (std::string(value) == "glfwGetTime()") {
                m_shader->setUniform1f(uniform.name, (float)glfwGetTime());
            }
        }
    }

    ImGui::End();
}

GUI::GUI(const Window& window)
: m_renderSpectrum(false)
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window.ptr(), true);
    ImGui_ImplOpenGL3_Init();
}

GUI::~GUI()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void GUI::mainMenu(const std::shared_ptr<Music> &music,
                   const std::shared_ptr<Renderer> &renderer,
                   float frameRate)
{
    static bool displayCustomShaderWindow = false;
    auto openMusicFile = [&]() {
        char filename[1024];
        FILE *f = popen(R"(zenity --file-selection  --file-filter=*.wav)", "r");
        fgets(filename, 1024, f);
        std::string filenameString = filename;
        filenameString.erase(std::remove(filenameString.begin(), filenameString.end(), '\n'), filenameString.cend());
        auto& songPath = music->songPath();
        if (!songPath.empty() && songPath == filenameString) {
            return;
        }
        music->setSongPath(filenameString);
        music->setLoaded(false);
    };

    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Open")) {
                std::thread t(openMusicFile);
                t.detach();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Visualization")) {
            if (ImGui::Selectable("Spectrum", m_renderSpectrum)) {
                toggleRenderSpectrum();
            }
            if (ImGui::Selectable("Custom Shader", displayCustomShaderWindow)) {
                displayCustomShaderWindow = !displayCustomShaderWindow;
            }
            ImGui::EndMenu();
        }
        ImGui::Text("Immediate Frame Rate: %.1f", frameRate);
        ImGui::EndMainMenuBar();
    }

    if (displayCustomShaderWindow) {
        m_shaderEditor.draw(renderer);
    }
}

void GUI::debug()
{
}

void GUI::render()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::newFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}
