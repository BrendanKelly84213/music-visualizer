//
// Created by brendan on 2/29/24.
//

#ifndef MUSIC_H
#define MUSIC_H

#include <SDL2/SDL_mixer.h>
#include <string>
#include <memory>
#include "Error.h"

class Music {
public:
    static Result<std::unique_ptr<Music>> create(const std::string& filename);
    Music() : m_ptr(nullptr)
    {}
    Music(_Mix_Music* ptr)
        : m_ptr(ptr)
    {}

    ~Music();
    bool load(const std::string& path);
    Mix_Music* ptr() { return m_ptr; }
private:

    Mix_Music* m_ptr {nullptr};
};

#endif //MUSIC_H
