//
// Created by brendan on 3/15/24.
//

#include "FFT.h"
Result<std::shared_ptr<FFT>> FFT::create(size_t dataBlockSize, const std::shared_ptr<SoundData> &soundData)
{
    auto in = fftw_alloc_complex(dataBlockSize);
    if (in == nullptr) {
        return Error("Failed to allocate memory for fftw in pointer");
    }
    auto out = fftw_alloc_complex(dataBlockSize);
    if (out == nullptr) {
        return Error("Failed to allocate memory for fftw out pointer");
    }
    auto plan = fftw_plan_dft_1d(static_cast<int>(dataBlockSize), in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    if (plan == nullptr) {
        return Error("Failed to allocate memory for fftw plan pointer");
    }
    return std::make_shared<FFT>(dataBlockSize, soundData, in, out, plan);
}

FFT::~FFT()
{
    fftw_free(m_out);
    fftw_free(m_in);
    fftw_destroy_plan(m_plan);
}

void FFT::execute(size_t numSamples)
{
    for (size_t i = 0; i < 2; ++i) {
        std::fill(m_in[i], m_in[i] + m_dataBlockSize, 0.0);
        std::fill(m_out[i], m_out[i] + m_dataBlockSize, 0.0);
    }
    // Collect sound data for fft
    for (size_t in_index = 0; in_index < numSamples && in_index < m_dataBlockSize && m_dataIndex < m_soundData->count(); ++in_index) {
        m_in[in_index][0] = m_soundData->at(m_dataIndex);
        m_in[in_index][1] = 0;
        m_dataIndex += m_soundData->info().channels;
    }

    fftw_execute(m_plan);
}

