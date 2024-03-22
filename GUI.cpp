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
: m_renderSpectrum(false), m_renderNoise(false)
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

void GUI::mainMenu(const std::shared_ptr<Music>& music, float frameRate)
{
    auto openFile = [&]() {
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
                std::thread t(openFile);
                t.detach();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Visualization")) {
            // FIXME: This is the worst shit I've ever done! Very bad!
            if (ImGui::Selectable("Spectrum", m_renderSpectrum)) {
                if (m_renderNoise)
                    toggleNoise();
                toggleRenderSpectrum();
            }
            if (ImGui::Selectable("Noise", m_renderNoise)) {
                if (m_renderSpectrum)
                    toggleRenderSpectrum();
                toggleNoise();
            }
            ImGui::EndMenu();
        }
        ImGui::Text("Immediate Frame Rate: %.1f", frameRate);
        ImGui::EndMainMenuBar();
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
