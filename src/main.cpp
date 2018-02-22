#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

#include "portaudiosystem.h"

#include "snowboy-detect-cxx-compat.h"

int main() {
  PortAudioSystem::init();

  std::string resourceFileName = "./resources/common.res";
  std::string modelFileName = "./resources/seattle.pmdl";

  Snowboy detector(resourceFileName.c_str(), modelFileName.c_str());
  detector.SetSensitivity("0.5");
  detector.SetAudioGain(1.0f);
  detector.ApplyFrontend(true);

  int bitsPerSample = detector.BitsPerSample();
  int numChannels = detector.NumChannels();
  int sampleRate = detector.SampleRate();
  int numHotWords = detector.NumHotWords();

  std::clog << "BitsPerSample: " << bitsPerSample << std::endl;
  std::clog << "NumChannels: " << numChannels << std::endl;
  std::clog << "SampleRate: " << sampleRate << std::endl;

  std::clog << "NumHotWords: " << numHotWords << std::endl;

  PortAudioSystem portAudio;

  portAudio.createStream(
    bitsPerSample,
    numChannels,
    sampleRate
  );

  portAudio.startStream();

  std::vector<int16_t> buffer;
  while(true) {
    portAudio.read(&buffer);
    if (buffer.size() != 0) {
      int result = detector.RunDetection(buffer.data(), buffer.size());
      std::cout << result << std::endl;
    }
  }

  portAudio.stopStream();

  PortAudioSystem::destroy();
}
