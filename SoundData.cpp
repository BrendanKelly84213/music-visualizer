//
// Created by brendan on 3/2/24.
//

#include <sndfile.h>
#include "SoundData.h"

SoundData::SoundData(const int64_t count, SNDFILE* sndfile)
{
    m_count = count;
    m_buffer = std::make_unique<float[]>(m_count);
    m_frames = sf_read_float(sndfile, m_buffer.get(), m_count);
    sf_close(sndfile);
}

std::optional<SoundData> SoundData::create(const std::string& filepath)
{
    SF_INFO info;
    SNDFILE* sndfile = sf_open(filepath.c_str(), SFM_READ, &info);
    if (sndfile == nullptr) {
        return {};
    }
    auto const count = info.frames * info.channels;
    return std::optional<SoundData>(SoundData(count, sndfile));
}