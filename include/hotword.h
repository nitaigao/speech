#ifndef _HOTWORD_H_
#define _HOTWORD_H_

#include "snowboy-detect-cxx-compat.h"
#include "vad.h"

class Hotword {

public:

  Hotword(const std::string& modelFile);

  bool init(const std::string& sensitivity, float audioGain);

  int bitsPerSample() const;

  int numChannels() const;

  int sampleRate() const;

  int detect(const int16_t* const data, const int array_length);

private:

  SnowboyDetector detector_;
  Vad vad_;

};

#endif
