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

class FloatDataView {
public:
    FloatDataView(const float* start, size_t size) : m_start(start), m_size(size)
    {}

    size_t size() const { return m_size; }

    const float& operator[](size_t index) const { return m_start[index]; }

    const float* begin() const { return m_start; }
    const float* end() const { return m_start + m_size; }

private:
    const float* m_start;
    size_t m_size;
};

class SoundData {
public:
    // NOTE: Experimental
    static std::variant<Error, SoundData> create(const std::string& filepath);

    std::vector<float> fft(size_t start, size_t count);
    FloatDataView window(size_t start, size_t count);

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
