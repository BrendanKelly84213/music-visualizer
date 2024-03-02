//
// Created by brendan on 3/2/24.
//

#ifndef SOUNDDATA_H
#define SOUNDDATA_H

#include <memory>
#include <optional>
#include <string>

class SoundData {
public:
    static std::optional<SoundData> create(const std::string& filepath);

    SoundData() = delete;

    float at(unsigned int i) const { return m_buffer[i]; }
    float* ptr() const { return m_buffer.get(); }
    int64_t frames() const { return m_frames; }
    int64_t count() const { return m_count; }
private:
    SoundData(int64_t count, SNDFILE* sndfile);

    std::unique_ptr<float[]> m_buffer {};
    int64_t m_frames {};
    int64_t m_count {};
};


#endif //SOUNDDATA_H
