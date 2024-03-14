//
// Created by brendan on 2/29/24.
//

#include "Music.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>

Result<std::unique_ptr<Music>> Music::create(const std::string &filename)
{
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        return Error("Failed to initialize SDL " + std::string(SDL_GetError()) + '\n');
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        return Error("Failed to initialize SDL_mixer " + std::string(Mix_GetError()) + '\n');
    }

    auto raw_ptr = Mix_LoadMUS(filename.c_str());
    if (raw_ptr == nullptr) {
        return Error("Failed to load music file " + std::string(Mix_GetError()) + '\n');
    }
    auto ptr = std::unique_ptr<Mix_Music, decltype(&Mix_FreeMusic)>(raw_ptr, Mix_FreeMusic);

    auto soundData = TRY(SoundData::create(filename), Error("Failed to load sound data"));

    return std::make_unique<Music>(std::move(ptr), std::make_shared<SoundData>(std::move(soundData)));
}


bool Music::load(const std::string& filename)
{
    auto raw_ptr = Mix_LoadMUS(filename.c_str());
    if (raw_ptr == nullptr) {
        return false;
    }
    m_ptr = std::unique_ptr<Mix_Music, decltype(&Mix_FreeMusic)>(raw_ptr, Mix_FreeMusic);
    return true;
}
