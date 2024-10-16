//
// Created by brendan on 3/21/24.
//

#include <thread>
#include <algorithm>
#include <stdexcept>
#include <filesystem>
#include "GUI.h"
#include "imgui.h"
#include "imnodes.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"

namespace fs = std::filesystem;

void GUI::init(const Window &window, std::shared_ptr<FrameBuffer> const& framebuffer)
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImNodes::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window.ptr(), true);
    ImGui_ImplOpenGL3_Init();

    ImGui::CreateContext();

    m_framebuffer = framebuffer;
}

GUI::~GUI()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    ImNodes::DestroyContext();
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

    onFrame();
}

void GUI::onFrame()
{
    m_nodeGraph.onFrame();

    // Output window
    ImGui::Begin("output window");
    {
        auto size = ImGui::GetContentRegionAvail();
        unsigned int colorBufferId = m_framebuffer->textureColorBuffer();
        m_framebuffer->rescale((int)size.x, (int)size.y);
        ImGui::Image((ImTextureID)colorBufferId, size);
        ImGui::End();
    }
}
