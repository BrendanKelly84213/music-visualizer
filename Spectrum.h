//
// Created by brendan on 3/10/24.
//

#ifndef SPECTRUM_H
#define SPECTRUM_H


#include <glm/vec2.hpp>
#include <memory>
#include <vector>
#include "Rectangle.h"
#include "FloatDataView.h"

class Spectrum {
public:
    static std::shared_ptr<Spectrum> create(unsigned int samples, const glm::vec2& resolution, const glm::vec4& color);

    Spectrum(unsigned int samples, const glm::vec2& resolution, std::vector<Rectangle>&& rectangles)
    : m_samples(samples), m_resolution(resolution), m_rectangles(std::move(rectangles))
    {}

    void setMagnitudes(FloatDataView magnitudes);
    void setResolution(const glm::vec2& resolution);
    void draw();
private:
    unsigned int m_samples {};
    glm::vec2 m_resolution {};
    std::vector<Rectangle> m_rectangles {};
    FloatDataView m_magnitudes {};
};


#endif //SPECTRUM_H
