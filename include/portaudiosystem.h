#include <portaudio.h>
#include <pa_ringbuffer.h>
#include <vector>

typedef char SAMPLE;

typedef struct
{
  int          frameIndex;
  int          maxFrameIndex;
  SAMPLE      *recordedSamples;
}
paTestData;

class PortAudioSystem {

public:

  PortAudioSystem();

  static int init();

  static int destroy();

  int createStream(int bitsPerSample, int numChannels, int sampleRate);

  int startStream();

  int stopStream();

  void read(std::vector<int16_t>* buffer);

  void Callback(const void* inputBuffer, unsigned long framesPerBuffer);

private:

  char* ringBufferStorage;

  PaStream* stream;
  PaUtilRingBuffer ringBuffer;

  int minReadSamples;
  int numLostSamples;
};
