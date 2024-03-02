//
// Created by brendan on 3/2/24.
//

#ifndef SOUNDDATA_H
#define SOUNDDATA_H

#include <memory>
#include <optional>
#include <string>
#include "Error.h"

class SoundData {
public:
    static std::variant<Error, SoundData> create(const std::string& filepath);

    float at(unsigned int i) const { return m_buffer[i]; }
    float* ptr() const { return m_buffer.get(); }
    int64_t frames() const { return m_frames; }
    int64_t count() const { return m_count; }
    SF_INFO const& info() const { return m_info; }
private:
    SoundData(int64_t count, SNDFILE* sndfile, const SF_INFO& info);

    std::unique_ptr<float[]> m_buffer {};
    int64_t m_frames {};
    int64_t m_count {};
    SF_INFO m_info {};
};


#endif //SOUNDDATA_H
