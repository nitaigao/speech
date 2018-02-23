#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

#include "portaudiosystem.h"

#include "snowboy-detect-cxx-compat.h"
#include "snowboy-vad-cxx-compat.h"

int main() {
  PortAudioSystem::init();

  std::string resourceFileName = "./resources/common.res";
  std::string modelFileName = "./resources/seattle.pmdl";

  SnowboyVad vad(resourceFileName.c_str());
  vad.SetAudioGain(1.0f);

  SnowboyDetector detector(resourceFileName.c_str(), modelFileName.c_str());
  detector.SetSensitivity("0.5");
  detector.SetAudioGain(1.0f);

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

  int frame = 0;

  std::vector<int16_t> buffer;
  while(true) {
    portAudio.read(&buffer);
    if (buffer.size() != 0) {
      int detectResult = detector.RunDetection(buffer.data(), buffer.size());
      bool hotwordDetected = detectResult > 0;

      int vadResult = vad.RunVad(buffer.data(), buffer.size());
      if (vadResult == 0) {
        std::cout << "Voice detected " << frame++ << std::endl;
      }

      bool voiceDetected = vadResult == 0;
      if (voiceDetected && hotwordDetected) {
        std::cout << "Hotword detected" << std::endl;
      }
    }
  }

  portAudio.stopStream();

  PortAudioSystem::destroy();
}
