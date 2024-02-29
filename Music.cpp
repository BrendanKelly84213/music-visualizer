//
// Created by brendan on 2/29/24.
//

#include "Music.h"

bool Music::load(const std::string& path)
{
    m_ptr = Mix_LoadMUS(path.c_str());
    if (m_ptr == nullptr) {
        return false;
    }
    return true;
}

Music::~Music()
{
    Mix_FreeMusic(m_ptr);
    m_ptr = nullptr;
}