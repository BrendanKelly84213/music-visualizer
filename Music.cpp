//
// Created by brendan on 2/29/24.
//

#include "Music.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <cassert>

Result<std::shared_ptr<FFT>> FFT::create(size_t dataBlockSize)
{
    auto in = fftw_alloc_complex(dataBlockSize);
    if (in == nullptr) {
        return Error("Failed to allocate memory for fftw in pointer");
    }
    auto out = fftw_alloc_complex(dataBlockSize);
    if (out == nullptr) {
        return Error("Failed to allocate memory for fftw out pointer");
    }
    auto plan = fftw_plan_dft_1d(static_cast<int>(dataBlockSize), in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    if (plan == nullptr) {
        return Error("Failed to allocate memory for fftw plan pointer");
    }
    return std::make_shared<FFT>(dataBlockSize, in, out, plan);
}

FFT::~FFT()
{
    fftw_free(m_out);
    fftw_free(m_in);
    fftw_destroy_plan(m_plan);
}

void FFT::execute(size_t numSamples, const SoundData& soundData)
{
   assert(m_in != nullptr);
   assert(m_out != nullptr);

    for (size_t i = 0; i < 2; ++i) {
        std::fill(m_in[i], m_in[i] + m_dataBlockSize, 0.0);
        std::fill(m_out[i], m_out[i] + m_dataBlockSize, 0.0);
    }
    // Collect sound data for fft
    for (size_t in_index = 0; in_index < numSamples && in_index < m_dataBlockSize && m_dataIndex < soundData.count(); ++in_index) {
        m_in[in_index][0] = soundData.at(m_dataIndex);
        m_in[in_index][1] = 0;
        m_dataIndex += soundData.info().channels;
    }

    fftw_execute(m_plan);
}

Result<std::shared_ptr<Music>> Music::create(size_t dataBlockSize)
{
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        return Error("Failed to initialize SDL " + std::string(SDL_GetError()) + '\n');
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        return Error("Failed to initialize SDL_mixer " + std::string(Mix_GetError()) + '\n');
    }

    auto music = std::make_shared<Music>();
    music->m_fft = TRY(FFT::create(dataBlockSize), Error("<- failed to create fft"));
    return music;
}

bool Music::load(const std::string& filename)
{
    auto raw_ptr = Mix_LoadMUS(filename.c_str());
    if (raw_ptr == nullptr) {
        return false;
    }
    m_fft->resetDataIndex();
    m_ptr = std::shared_ptr<Mix_Music>(raw_ptr, Mix_FreeMusic);
    m_soundData = TRY(SoundData::create(filename), false);
    m_loaded = true;
    return true;
}
