#pragma once

#include <cstdint>

#include "snowboy-detect.h"

class SnowboyVad {

 public:

  SnowboyVad(const char* resourceFileName);

  void SetAudioGain(float audioGain) const;

  void ApplyFrontend(bool applyFrontend) const;

  int SampleRate() const;

  int NumChannels() const;

  int BitsPerSample() const;

  int RunVad(const int16_t* data, int numSamples, bool isEnd = false);

 private:

  void* detector_;
};
