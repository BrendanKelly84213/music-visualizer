//
// Created by brendan on 3/15/24.
//

#ifndef FFT_H
#define FFT_H


#include <cstddef>
#include <fftw3.h>
#include <complex>
#include <iostream>
#include "SoundData.h"
class FFT {
public:
    static Result<std::shared_ptr<FFT>> create(size_t dataBlockSize, const std::shared_ptr<SoundData>& soundData);
    FFT(size_t dataBlockSize, const std::shared_ptr<SoundData>& soundData, fftw_complex* in, fftw_complex* out, fftw_plan_s* plan)
        : m_dataBlockSize(dataBlockSize), m_soundData(soundData), m_in(in), m_out(out), m_plan(plan), m_dataIndex(0)
    {}

    FFT(const FFT& fft) = delete;

    ~FFT();
    void execute(size_t numSamples);
    [[nodiscard]] double magnitudeAt(size_t i) const { return std::sqrt(m_out[i][0] * m_out[i][0] + m_out[i][1] * m_out[i][1]); }
private:

    size_t m_dataBlockSize {};
    std::shared_ptr<SoundData> m_soundData;
    fftw_complex* m_in;
    fftw_complex* m_out;
    fftw_plan_s* m_plan;
    size_t m_dataIndex {0};
};


#endif //FFT_H
