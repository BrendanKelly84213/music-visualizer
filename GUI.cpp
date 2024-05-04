//
// Created by brendan on 3/21/24.
//

#include <memory>
#include <thread>
#include <algorithm>
#include <stdexcept>
#include <filesystem>
#include "GUI.h"
#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"
#include "config.h"

namespace fs = std::filesystem;

void WAV::draw()
{
    auto openMusicFile = [&]() {
        char filename[1024];
        FILE *f = popen(R"(zenity --file-selection  --file-filter=*.wav)", "r");
        fgets(filename, 1024, f);
        std::string filenameString = filename;
        filenameString.erase(std::remove(filenameString.begin(), filenameString.end(), '\n'), filenameString.cend());
        auto& songPath = m_music->songPath();
        if (!songPath.empty() && songPath == filenameString) {
            return;
        }
        m_music->setSongPath(filenameString);
        m_music->setLoaded(false);
    };

    ImGui::Begin(m_name.c_str());
    if (ImGui::Button("Select WAV File")) {
        std::thread t(openMusicFile);
        t.detach();
    }
    ImGui::Text("%s", m_music->songPath().c_str());
    if (!m_music->loaded() && !m_music->songPath().empty()) {
        if (!m_music->load()) {
            ImGui::Text("Could not load wav file at: %s", m_music->songPath().c_str());
        }
    }
    if (ImGui::Button("Play")) {
        if (m_music->paused() && m_music->loaded()) {
            m_music->resume();
        }
        if (!Music::playing() && m_music->loaded()) {
            m_music->play();
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("Pause")) {
        if (Music::playing() && m_music->loaded()) {
            m_music->pause();
        }
    }

    ImGui::End();
}

void SpectrumEditor::draw(const std::shared_ptr<Renderer>& renderer, const std::shared_ptr<Music>& music)
{
    ImGui::Begin("Spectrum");
    ImGui::End();
}

void ShaderEditor::draw(const std::shared_ptr<Renderer>& renderer, const std::shared_ptr<Music>& music)
{
    // FIXME: Only ubuntu compatible at the moment
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
        // FIXME: This method of maintaining button to modal ID relation is not pretty, and kind of doesn't work
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
        // FIXME: uniform.type and uniform.inputMethod should be an enum or something similar and should be accessed by drop down menu
        if (uniform.type == "float") {
            if (uniform.inputMethod == "manual") {
                float value = std::get<float>(uniform.value);
                ImGui::DragFloat(std::string("Value " + uniform.name).c_str(), &value);
                uniform.value = value;
                if (m_shader == nullptr) {
                    continue;
                }
                m_shader->setUniform1f(uniform.name, value);
            } else if (uniform.inputMethod == "function") {
                char value[128];
                strcpy(value, uniform.function.c_str());
                ImGui::InputTextWithHint(std::string("Function Name " + uniform.name).c_str(), "glfwGetTime()", value, IM_ARRAYSIZE(value));
                uniform.function = value;
                if (m_shader == nullptr) {
                    continue;
                }
                if (std::string(value) == "glfwGetTime()") {
                    uniform.value = (float)glfwGetTime();
                    m_shader->setUniform1f(uniform.name, (float)glfwGetTime());
                } else if (std::string(value) == "fftAverage()") {
                    uniform.value = (float)music->fftAverage();
                    m_shader->setUniform1f(uniform.name, (float)music->fftAverage());
                }
                ImGui::Text("%f", std::get<float>(uniform.value));
            }
        }
    }

    ImGui::End();
}



void GUI::init(const Window &window)
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

    // FIXME: HardCoding like this is no good...
    m_nodes["Time"] = std::make_shared<Time>();
    m_nodes["WAV"] = WAV::create();

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
    static bool displaySpectrumEditor = false;

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
                displaySpectrumEditor = !displaySpectrumEditor;
            }
            if (ImGui::Selectable("Custom Shader", displayCustomShaderWindow)) {
                displayCustomShaderWindow = !displayCustomShaderWindow;
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Add Node")) {
            if (ImGui::MenuItem("Time")) {
                addNode("Time");
            }
            if (ImGui::MenuItem("WAV")) {
                addNode("WAV");
            }
            ImGui::EndMenu();
        }
        ImGui::Text("Immediate Frame Rate: %.1f", frameRate);
        ImGui::EndMainMenuBar();
    }

    if (displaySpectrumEditor) {
        m_spectrumEditor.draw(renderer, music);
    }

    if (displayCustomShaderWindow) {
        m_shaderEditor.draw(renderer, music);
    }

    for (const auto& node : m_nodes) {
        if (node.second != nullptr && node.second->shouldBeDrawn()) {
            node.second->draw();
        }
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

void GUI::addNode(const std::string& name)
{
    m_nodes[name]->setShouldBeDrawn(true);
}

