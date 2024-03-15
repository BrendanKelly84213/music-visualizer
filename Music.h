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
    Music(std::unique_ptr<Mix_Music, decltype(&Mix_FreeMusic)>&& ptr, std::shared_ptr<SoundData> soundData)
        : m_ptr(std::move(ptr)), m_soundData(std::move(soundData))
    {}

    ~Music() { Mix_CloseAudio(); }

    bool load(const std::string& filename);
    static bool playing() { return Mix_PlayingMusic(); }
    void play(int loops = -1) { Mix_PlayMusic(m_ptr.get(), loops); }
    [[nodiscard]] std::shared_ptr<SoundData> data() const { return m_soundData; }
private:
    std::shared_ptr<SoundData> m_soundData {};
    std::unique_ptr<Mix_Music, decltype(&Mix_FreeMusic)> m_ptr;
};

#endif //MUSIC_H
