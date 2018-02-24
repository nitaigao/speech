#include "vad.h"

#include <iostream>

Vad::Vad()
  : fvad_(fvad_new()) {

}

bool Vad::setSampleRate(int sampleRate) const {
  int result = fvad_set_sample_rate(fvad_, sampleRate);
  bool success = result == 0;
  return success;
}

bool Vad::setMode(int mode) const {
  int result = fvad_set_mode(fvad_, mode);
  bool success = (result == 0);
  return success;
}

int Vad::analyze(const int16_t* const buffer, const int bufferSize) const {
  const int16_t * temp = buffer;

  bool fVadDetectedVoice = false;

  for(int i = 0; i < bufferSize; i += 160) {
    int voiceActive = fvad_process(fvad_, temp, 160); // 10 ms window

    if (voiceActive == -1) {
      std::cerr << "VAD error" << std::endl;
    }

    fVadDetectedVoice = fVadDetectedVoice || voiceActive == 1;

    temp = temp + 160; // processed 160 samples (320 bytes)

    std::clog << "Detected voice: " << voiceActive << std::endl;
  }

  if (fVadDetectedVoice) {
    return 1;
  }

  return 0;
}
