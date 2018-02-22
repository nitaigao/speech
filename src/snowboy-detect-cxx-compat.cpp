#undef _GLIBCXX_USE_CXX11_ABI
#define _GLIBCXX_USE_CXX11_ABI 0

#include "snowboy-detect-cxx-compat.h"

static snowboy::SnowboyDetect* c(void* p) {
  return reinterpret_cast<snowboy::SnowboyDetect*>(p);
}

static const snowboy::SnowboyDetect* c(const void* p) {
  return reinterpret_cast<const snowboy::SnowboyDetect*>(p);
}

Snowboy::Snowboy(const char* resourceFileName, const char* modelFileName)
    : detector_(new snowboy::SnowboyDetect(resourceFileName, modelFileName)) {
}

void Snowboy::SetSensitivity(const char* sensitivity) const {
  c(detector_)->SetSensitivity(sensitivity);
}

void Snowboy::ApplyFrontend(bool applyFrontend) const {
  c(detector_)->ApplyFrontend(applyFrontend);
}

void Snowboy::SetAudioGain(float audioGain) const {
  c(detector_)->SetAudioGain(audioGain);
}

int Snowboy::NumHotWords() const {
  return c(detector_)->NumHotwords();
}

int Snowboy::SampleRate() const {
  return c(detector_)->SampleRate();
}

int Snowboy::NumChannels() const {
  return c(detector_)->NumChannels();
}

int Snowboy::BitsPerSample() const {
  return c(detector_)->BitsPerSample();
}

int Snowboy::RunDetection(const int16_t* data, int num_samples) {
  return c(detector_)->RunDetection(data, num_samples);
}
