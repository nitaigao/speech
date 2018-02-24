#ifndef __VAD_H_
#define __VAD_H_

extern "C" {
  #include "fvad.h"
}

class Vad {

public:

  static const int MODE_AGGRESSIVE = 3;

public:

  Vad();

  bool setSampleRate(int sampleRate) const;

  bool setMode(int mode) const;

  int analyze(const int16_t* const buffer, const int bufferSize) const;

private:

  Fvad* fvad_;

};

#endif
