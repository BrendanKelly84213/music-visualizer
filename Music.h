//
// Created by brendan on 2/29/24.
//

#ifndef MUSIC_H
#define MUSIC_H

#include <SDL2/SDL_mixer.h>
#include <string>

class Music {
public:
    Music() : m_ptr(nullptr)
    {
    }
    ~Music();

    bool load(const std::string& path);
    Mix_Music* ptr() { return m_ptr; }
private:
    Mix_Music* m_ptr;
};


#endif //MUSIC_H
