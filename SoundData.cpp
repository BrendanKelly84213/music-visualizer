//
// Created by brendan on 3/2/24.
//

#include <sndfile.h>
#include <variant>
#include <cstring>
#include "SoundData.h"

SoundData::SoundData(const int64_t count, SNDFILE* sndfile, const SF_INFO& info)
: m_count(count), m_info(info)
{
    m_buffer = std::make_unique<float[]>(m_count);
    m_frames = sf_read_float(sndfile, m_buffer.get(), m_count);
    sf_close(sndfile);
}

std::variant<Error, SoundData> SoundData::create(const std::string& filepath)
{
    SF_INFO info {};
    SNDFILE* sndfile = sf_open(filepath.c_str(), SFM_READ, &info);
    if (sndfile == nullptr) {
        return Error(sf_strerror(sndfile));
    }
    auto const count = info.frames * info.channels;
    return SoundData(count, sndfile, info);
}

FloatDataView SoundData::window(size_t start, size_t count)
{
    return {m_buffer.get() + start, count};
}

std::vector<float> SoundData::fft(size_t start, size_t count)
{
    auto dataView = window(start, count);
    std::vector<float> returnVector;
    for (auto const& value : dataView) {
        returnVector.push_back(value);
    }
    return returnVector;
}
