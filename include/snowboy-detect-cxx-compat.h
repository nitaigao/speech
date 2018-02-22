#pragma once

#include <cstdint>

#include "snowboy-detect.h"

class Snowboy {

 public:

  Snowboy(const char* resourceFileName, const char* modelFileName);

  void SetSensitivity(const char* sensitivity) const;

  void SetAudioGain(float audioGain) const;

  void ApplyFrontend(bool applyFrontend) const;

  int SampleRate() const;

  int NumChannels() const;

  int BitsPerSample() const;

  int NumHotWords() const;

  int RunDetection(const int16_t* data, int num_samples);

 private:

  void* detector_;
};
