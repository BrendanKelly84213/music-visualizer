//
// Created by brendan on 3/2/24.
//

#include "SoundData.h"

SoundData::SoundData(const int64_t count, SNDFILE* sndfile, const SF_INFO& info)
: m_count(count), m_info(info)
{
    m_buffer = std::make_unique<double[]>(m_count);
    m_frames = sf_read_double(sndfile, m_buffer.get(), m_count);
    sf_close(sndfile);
    
}

Result<SoundData> SoundData::create(const std::string& filepath)
{
    SF_INFO info {};
    SNDFILE* sndfile = sf_open(filepath.c_str(), SFM_READ, &info);
    if (sndfile == nullptr) {
        return Error(sf_strerror(sndfile));
    }
    auto const count = info.frames * info.channels;
    return SoundData(count, sndfile, info);
}
