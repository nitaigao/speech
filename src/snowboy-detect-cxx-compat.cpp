#undef _GLIBCXX_USE_CXX11_ABI
#define _GLIBCXX_USE_CXX11_ABI 0

#include "snowboy-detect-cxx-compat.h"

static snowboy::SnowboyDetect* c(void* p) {
  return reinterpret_cast<snowboy::SnowboyDetect*>(p);
}

static const snowboy::SnowboyDetect* c(const void* p) {
  return reinterpret_cast<const snowboy::SnowboyDetect*>(p);
}

SnowboyDetector::SnowboyDetector(const char* resourceFileName, const char* modelFileName)
    : detector_(new snowboy::SnowboyDetect(resourceFileName, modelFileName)) {
}

void SnowboyDetector::SetSensitivity(const char* sensitivity) const {
  c(detector_)->SetSensitivity(sensitivity);
}

void SnowboyDetector::ApplyFrontend(bool applyFrontend) const {
  c(detector_)->ApplyFrontend(applyFrontend);
}

void SnowboyDetector::SetAudioGain(float audioGain) const {
  c(detector_)->SetAudioGain(audioGain);
}

int SnowboyDetector::NumHotWords() const {
  return c(detector_)->NumHotwords();
}

int SnowboyDetector::SampleRate() const {
  return c(detector_)->SampleRate();
}

int SnowboyDetector::NumChannels() const {
  return c(detector_)->NumChannels();
}

int SnowboyDetector::BitsPerSample() const {
  return c(detector_)->BitsPerSample();
}

int SnowboyDetector::RunDetection(const int16_t* data, int num_samples) {
  return c(detector_)->RunDetection(data, num_samples);
}
