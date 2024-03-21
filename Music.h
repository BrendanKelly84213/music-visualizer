//
// Created by brendan on 2/29/24.
//

#ifndef MUSIC_H
#define MUSIC_H

#include <SDL2/SDL_mixer.h>
#include <string>
#include <memory>
#include <fftw3.h>
#include "Error.h"
#include "SoundData.h"

class FFT {
public:
    static Result<std::shared_ptr<FFT>> create(size_t dataBlockSize);
    FFT(size_t dataBlockSize, fftw_complex* in, fftw_complex* out, fftw_plan_s* plan)
        : m_dataBlockSize(dataBlockSize), m_in(in), m_out(out), m_plan(plan), m_dataIndex(0)
    {}
    FFT() = delete;
    FFT(const FFT& fft) = delete;

    ~FFT();
    void execute(size_t numSamples, const SoundData& soundData);
    void resetDataIndex() { m_dataIndex = 0; }
    [[nodiscard]] double magnitudeAt(size_t i) const { return std::sqrt(m_out[i][0] * m_out[i][0] + m_out[i][1] * m_out[i][1]); }
private:

    size_t m_dataBlockSize {};
    fftw_complex* m_in {};
    fftw_complex* m_out {};
    fftw_plan_s* m_plan {};
    size_t m_dataIndex {0};
};

class Music {
public:
    static Result<std::shared_ptr<Music>> create(size_t dataBlockSize);
    Music()
    : m_loaded(false), m_ptr(nullptr, &Mix_FreeMusic), m_fft(nullptr)
    {}

    ~Music() { Mix_CloseAudio(); }

    void executeFFT(size_t numSamples) { m_fft->execute(numSamples, m_soundData); }
    bool load();

    static bool playing() { return Mix_PlayingMusic(); }
    void play(int loops = -1) { Mix_PlayMusic(m_ptr.get(), loops); }
    void setLoaded(bool loaded) { m_loaded = loaded; }
    void setSongPath(const std::string& songPath) { m_songPath = songPath; }
    [[nodiscard]] double fftMagnitudeAt(size_t i) const { return m_fft->magnitudeAt(i); }
    [[nodiscard]] bool loaded() const { return m_loaded; }
    [[nodiscard]] SF_INFO const& info() const { return m_soundData.info(); }
    [[nodiscard]] std::string const& songPath() const { return m_songPath; }
private:
    SoundData m_soundData;
    std::shared_ptr<Mix_Music> m_ptr {};
    std::shared_ptr<FFT> m_fft {};
    bool m_loaded;
    std::string m_songPath {};
};

#endif //MUSIC_H
