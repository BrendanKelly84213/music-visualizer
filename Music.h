//
// Created by brendan on 2/29/24.
//

#ifndef MUSIC_H
#define MUSIC_H

#include <SDL2/SDL_mixer.h>
#include <string>
#include <memory>
#include "Error.h"
#include "SoundData.h"

class Music {
public:
    static Result<std::unique_ptr<Music>> create(const std::string& filename);
    Music() : m_ptr(nullptr)
    {}
    Music(_Mix_Music* ptr, std::shared_ptr<SoundData> soundData)
        : m_ptr(ptr), m_soundData(std::move(soundData))
    {}

    ~Music();
    bool load(const std::string& path);
    static bool playing() { return Mix_PlayingMusic(); }
    void play(int loops = -1) { Mix_PlayMusic(m_ptr, loops); }
    Mix_Music* ptr() { return m_ptr; }
    [[nodiscard]] std::shared_ptr<SoundData> data() const { return m_soundData; }
private:
    std::shared_ptr<SoundData> m_soundData {};
    Mix_Music* m_ptr {nullptr};
};

#endif //MUSIC_H
