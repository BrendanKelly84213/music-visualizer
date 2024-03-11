//
// Created by brendan on 3/10/24.
//

#include "Spectrum.h"

#include <utility>
#include <iostream>

std::shared_ptr<Spectrum> Spectrum::create(const unsigned int samples, const glm::vec2& resolution, const glm::vec4& color)
{
    std::vector<Rectangle> rectangles;
    float width = 2.0f / static_cast<float>(samples);
    float height = 2.0f;
    for (size_t i = 0; i < samples; ++i) {
        auto position = glm::vec2((static_cast<double>(i) * width - 2.0) * 2.0f, -1);
        auto rectangle = Rectangle::create(width, height, position, color);
        if (rectangle == nullptr) {
            std::cout << "Failed to create rectangle: " << '\n';
            return nullptr;
        }
        rectangles.push_back(*rectangle);
    }

    return std::make_shared<Spectrum>(Spectrum(samples, resolution, std::move(rectangles)));
}

void Spectrum::setMagnitudes(FloatDataView magnitudes)
{
    m_magnitudes = magnitudes;
}

void Spectrum::setResolution(const glm::vec2& resolution)
{
    m_resolution = resolution;
}

void Spectrum::draw()
{
    for (size_t i = 0; i < m_rectangles.size(); ++i) {
        std::cout << "i: " << i << " rectangle width " << m_rectangles[i].width() << "  rectangle height: " << m_rectangles[i].height() << " rectangle position x: " << m_rectangles[i].position().x << " y: " << m_rectangles[i].position().y << '\n';
        m_rectangles[i].draw();
    }
}
