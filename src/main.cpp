#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

#include "portaudiosystem.h"

extern "C" {
  #include "fvad.h"
}

#include "snowboy-detect-cxx-compat.h"
#include "snowboy-vad-cxx-compat.h"

int main() {
  PortAudioSystem::init();
  Fvad* fvad = fvad_new();

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

  int setfVadSampleRateResult = fvad_set_sample_rate(fvad, sampleRate);

  if (setfVadSampleRateResult == -1) {
    std::cout << "Unable to set fvad sample rate" << std::endl;
  }

  int setfVadModeResult = fvad_set_mode(fvad, 3);

  if (setfVadModeResult == -1) {
    std::cout << "Unable to set fvad mode" << std::endl;
  }

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
      const int16_t * temp = buffer.data();
      bool fVadDetectedVoice = false;
      for(int i = 0, ms = 0; i < buffer.size(); i += 160, ms += 10)
      {
        int isActive = fvad_process(fvad, temp, 160); //10 ms window
        fVadDetectedVoice = fVadDetectedVoice || isActive == 1;
        temp = temp + 160; // processed 160 samples (320 bytes)
      }
      if (hotwordDetected && fVadDetectedVoice) {
        std::clog << "hotword detected" << std::endl;
      }
    }
  }

  portAudio.stopStream();

  PortAudioSystem::destroy();
}
