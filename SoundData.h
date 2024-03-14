//
// Created by brendan on 3/2/24.
//

#ifndef SOUNDDATA_H
#define SOUNDDATA_H

#include <memory>
#include <optional>
#include <string>
#include <vector>
#include <sndfile.h>
#include "Error.h"
#include "FloatDataView.h"

class SoundData {
public:
    // NOTE: Experimental
    static Result<SoundData> create(const std::string& filepath);

    [[nodiscard]] double at(unsigned int i) const { return m_buffer[i]; }
    [[nodiscard]] double* ptr() const { return m_buffer.get(); }
    [[nodiscard]] int64_t frames() const { return m_frames; }
    [[nodiscard]] int64_t count() const { return m_count; }
    [[nodiscard]] SF_INFO const& info() const { return m_info; }

private:
    SoundData(int64_t count, SNDFILE* sndfile, const SF_INFO& info);

    std::unique_ptr<double[]> m_buffer {};
    int64_t m_frames {};
    int64_t m_count {};
    SF_INFO m_info {};
};


#endif //SOUNDDATA_H
