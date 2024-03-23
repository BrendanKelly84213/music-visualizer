//
// Created by brendan on 3/21/24.
//

#include "Spectrum.h"
#include "RenderCommand.h"

void Spectrum::render(size_t numSamplesShown, const std::shared_ptr<Music>& music, const std::shared_ptr<Renderer>& renderer, const glm::vec4& color)
{
    auto rectangleWidth = 2.0 / static_cast<double>(numSamplesShown);
    for (size_t i = 0; i < numSamplesShown; i++) {
        auto rectangleHeight = music->fftMagnitudeAt(i) * .01;
        renderer->drawQuad({rectangleWidth, rectangleHeight}, {(static_cast<double>(i) * rectangleWidth - 1.0), 0.0}, color);
        RenderCommand::drawIndexed(6);
    }
}
