#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

#include "portaudiosystem.h"
#include "hotword.h"

int main() {
  PortAudioSystem::init();

  std::string modelFileName = "./resources/seattle.pmdl";
  Hotword hotword(modelFileName);
  hotword.init("0.5", 1.0f);

  int bitsPerSample = hotword.bitsPerSample();
  int numChannels = hotword.numChannels();
  int sampleRate = hotword.sampleRate();

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
      int detectResult = hotword.detect(buffer.data(), buffer.size());
      if (detectResult == 1) {
        std::clog << "hotword detected" << std::endl;
      }
    }
  }

  portAudio.stopStream();

  PortAudioSystem::destroy();
}
