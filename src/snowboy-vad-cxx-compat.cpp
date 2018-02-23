#undef _GLIBCXX_USE_CXX11_ABI
#define _GLIBCXX_USE_CXX11_ABI 0

#include "snowboy-vad-cxx-compat.h"

static snowboy::SnowboyVad* c(void* p) {
  return reinterpret_cast<snowboy::SnowboyVad*>(p);
}

static const snowboy::SnowboyVad* c(const void* p) {
  return reinterpret_cast<const snowboy::SnowboyVad*>(p);
}

SnowboyVad::SnowboyVad(const char* resourceFileName)
    : detector_(new snowboy::SnowboyVad(resourceFileName)) {
}

void SnowboyVad::ApplyFrontend(bool applyFrontend) const {
  c(detector_)->ApplyFrontend(applyFrontend);
}

void SnowboyVad::SetAudioGain(float audioGain) const {
  c(detector_)->SetAudioGain(audioGain);
}

int SnowboyVad::SampleRate() const {
  return c(detector_)->SampleRate();
}

int SnowboyVad::NumChannels() const {
  return c(detector_)->NumChannels();
}

int SnowboyVad::BitsPerSample() const {
  return c(detector_)->BitsPerSample();
}

int SnowboyVad::RunVad(const int16_t* data, int numSamples, bool isEnd) {
  return c(detector_)->RunVad(data, numSamples, isEnd);
}
