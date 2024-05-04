//
// Created by brendan on 4/23/24.
//

#ifndef APPLICATION_H
#define APPLICATION_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <array>
#include <filesystem>
#include <algorithm>
#include <thread>
#include <glm/ext/matrix_transform.hpp>

#include "Window.h"
#include "Music.h"
#include "Renderer.h"
#include "SoundData.h"
#include "RenderCommand.h"
#include "GUI.h"
#include "Spectrum.h"
#include "imgui.h"
#include "PerlinNoise.h"
#include "config.h"


class Application {
public:
    Application()
    : m_window(1024, 800, "Visualizer")
    {
    }

    bool init();

    int run();
private:
    Window m_window;
    GUI m_gui;
    std::shared_ptr<Music> m_music;
    std::shared_ptr<Renderer> m_renderer;

};


#endif //APPLICATION_H
