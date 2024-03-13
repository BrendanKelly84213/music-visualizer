//
// Created by brendan on 3/2/24.
//

#ifndef SOUNDDATA_H
#define SOUNDDATA_H

#include <memory>
#include <optional>
#include <string>
#include <vector>
#include "Error.h"
#include "FloatDataView.h"

class SoundData {
public:
    // NOTE: Experimental
    static Result<SoundData> create(const std::string& filepath);

    std::vector<float> fft(size_t start, size_t count);
    FloatDataView window(size_t start, size_t count);

    [[nodiscard]] float at(unsigned int i) const { return m_buffer[i]; }
    [[nodiscard]] float* ptr() const { return m_buffer.get(); }
    [[nodiscard]] int64_t frames() const { return m_frames; }
    [[nodiscard]] int64_t count() const { return m_count; }
    [[nodiscard]] SF_INFO const& info() const { return m_info; }

private:
    SoundData(int64_t count, SNDFILE* sndfile, const SF_INFO& info);

    std::unique_ptr<float[]> m_buffer {};
    int64_t m_frames {};
    int64_t m_count {};
    SF_INFO m_info {};
};


#endif //SOUNDDATA_H
