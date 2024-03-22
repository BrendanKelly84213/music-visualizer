//
// Created by brendan on 3/21/24.
//

#ifndef SPECTRUM_H
#define SPECTRUM_H


#include <memory>
#include "Music.h"
#include "Renderer.h"

class Spectrum {
public:
    static void render(size_t numSamplesShown, const std::shared_ptr<Music>& music, const std::shared_ptr<Renderer>& renderer, const glm::vec4& color);
};


#endif //SPECTRUM_H
