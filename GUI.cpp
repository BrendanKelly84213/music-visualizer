//
// Created by brendan on 3/21/24.
//

#include <memory>
#include <thread>
#include <algorithm>
#include "GUI.h"
#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"

GUI::GUI(const Window& window)
: m_renderSpectrum(false), m_renderCustomShader(false)
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

// FIXME: I am in a rush for some reason and this needs to be refactored later
struct Uniform {
    std::string type;
    std::string name;
    std::string value;
};

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
        static std::string vertexFilename;
        static std::string fragmentFilename;
        auto openVertexShaderFile = [&]() {
            char filename[1024];
            FILE *f = popen(R"(zenity --file-selection  --file-filter=*.glsl)", "r");
            fgets(filename, 1024, f);
            std::string filenameString = filename;
            filenameString.erase(std::remove(filenameString.begin(), filenameString.end(), '\n'), filenameString.cend());
            vertexFilename = filenameString;
        };

        auto openFragmentShaderFile = [&]() {
            char filename[1024];
            FILE *f = popen(R"(zenity --file-selection  --file-filter=*.glsl)", "r");
            fgets(filename, 1024, f);
            std::string filenameString = filename;
            filenameString.erase(std::remove(filenameString.begin(), filenameString.end(), '\n'), filenameString.cend());
            fragmentFilename = filenameString;
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
        // FIXME: refactor static variable to something else
        static std::string message;
        if (ImGui::Button("Compile")) {
            // FIXME: shaders should get unique ID's
            auto result = renderer->loadShader("custom", vertexFilename, fragmentFilename);
            if (result.isError()) {
                message = "Failed to compile: " + result.error().message();
                std::cout << message << '\n';
            } else {
                message = "Compiled shader";
            }
        }
        if (ImGui::Selectable("Draw", m_renderCustomShader)) {
            toggleCustomShader();
        }
        ImGui::Text("%s", message.c_str());
        if (ImGui::Button("Add Uniform")) {
            ImGui::OpenPopup("Uniform Modal");
        }
        static std::vector<Uniform> uniforms {};
        if (ImGui::BeginPopupModal("Uniform Modal")) {
            static char uniformType[128] = "float";
            static char uniformName[128] = "u_time";
            static char uniformValue[128] = "0.0f";
            ImGui::InputTextWithHint("Type", "example: for uniform float u_time -> float", uniformType, IM_ARRAYSIZE(uniformType));
            ImGui::InputTextWithHint("Name", "example: for uniform float u_time -> u_time", uniformName, IM_ARRAYSIZE(uniformName));
            ImGui::InputTextWithHint("Value", "example: for uniform float u_time -> 0.0f", uniformValue, IM_ARRAYSIZE(uniformName));
            if (ImGui::Button("Create")) {
                uniforms.push_back({ .type = uniformType, .name = uniformName, .value = uniformValue });
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::Button("Close"))
                ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
        }
        for (auto& uniform : uniforms) {
            ImGui::Text("%s", std::string("uniform " + uniform.type + " " + uniform.name).c_str());
            char value[128] = "";
            memcpy(value, uniform.value.c_str(), uniform.value.size());
            if (uniform.type == "float") {
                ImGui::InputText(std::string("Value " + uniform.name).c_str(), value, IM_ARRAYSIZE(value));
                uniform.value = value;
            }
        }
        ImGui::End();
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
