#include "hotword.h"

#include <iostream>

#define RESOURCE_FILE "./resources/common.res"

Hotword::Hotword(const std::string& modelFile)
  : detector_(RESOURCE_FILE, modelFile.c_str()) {

}

bool Hotword::init(const std::string& sensitivity, float audioGain) {
  detector_.SetSensitivity(sensitivity.c_str());
  detector_.SetAudioGain(audioGain);

  int bitsPerSample = detector_.BitsPerSample();
  int numChannels = detector_.NumChannels();
  int sampleRate = detector_.SampleRate();

  std::clog << "BitsPerSample: " << bitsPerSample << std::endl;
  std::clog << "NumChannels: " << numChannels << std::endl;
  std::clog << "SampleRate: " << sampleRate << std::endl;

  bool result = 0;

  result = vad_.setSampleRate(sampleRate);

  if (!result) {
    std::cout << "Unable to set fvad sample rate" << std::endl;
    return false;
  }

  result = vad_.setMode(Vad::MODE_AGGRESSIVE);

  if (!result) {
    std::cout << "Unable to set fvad mode" << std::endl;
    return false;
  }

  return true;
}

int Hotword::bitsPerSample() const {
  return detector_.BitsPerSample();
}

int Hotword::numChannels() const {
  return detector_.NumChannels();
}

int Hotword::sampleRate() const {
  return detector_.SampleRate();
}

int Hotword::detect(const int16_t* const buffer, const int bufferSize) {
  int detectResult = detector_.RunDetection(buffer, bufferSize);
  bool hotwordDetected = detectResult > 0;

  int vadResult = vad_.analyze(buffer, bufferSize);
  bool voiceDetected = vadResult > 0;

  bool voiceMatch = hotwordDetected && voiceDetected;
  return voiceMatch;
}
