#include <iostream>
#include <pa_util.h>
#include "portaudiosystem.h"

#define FRAMES_PER_BUFFER (512)

static int recordCallback(const void* inputBuffer,
                          void* outputBuffer,
                          unsigned long framesPerBuffer,
                          const PaStreamCallbackTimeInfo* timeInfo,
                          PaStreamCallbackFlags statusFlags,
                          void* userData) {

  PortAudioSystem* portAudioSystem = reinterpret_cast<PortAudioSystem*>(userData);
  portAudioSystem->Callback(inputBuffer, framesPerBuffer);

  // if (outputBuffer != NULL) {
  //   float *input = (float*)inputBuffer;
  //   float *output = (float*)outputBuffer;

  //   for (unsigned int i = 0; i < framesPerBuffer; i++) {
  //     output[i] = input[i];
  //   }
  // }

  return paContinue;
}

PortAudioSystem::PortAudioSystem() {
  numLostSamples = 0;
  minReadSamples = 0;
}

void PortAudioSystem::Callback(const void* inputBuffer, unsigned long framesPerBuffer) {
  ring_buffer_size_t numWrittenSamples = PaUtil_WriteRingBuffer(&ringBuffer, inputBuffer, framesPerBuffer);
  numLostSamples += framesPerBuffer - numWrittenSamples;
}

void PortAudioSystem::read(std::vector<int16_t>* buffer) {
  ring_buffer_size_t numAvailableSamples = 0;

  if (numLostSamples > 0) {
    std::cerr << "Lost " << numLostSamples << " due to ring buffer overflow" << std::endl;
  }

  while (true) {
    numAvailableSamples = PaUtil_GetRingBufferReadAvailable(&ringBuffer);

    if (numAvailableSamples >= minReadSamples) {
      break;
    }

    Pa_Sleep(5);
  }

  numAvailableSamples = PaUtil_GetRingBufferReadAvailable(&ringBuffer);
  buffer->resize(numAvailableSamples);

  ring_buffer_size_t numReadSamples = PaUtil_ReadRingBuffer(&ringBuffer, buffer->data(), numAvailableSamples);

  if (numReadSamples != numAvailableSamples) {
    std::cerr << numAvailableSamples << " samples were available but " << numReadSamples << " samples were read" << std::endl;
  }
}

int PortAudioSystem::init() {
  PaError err = Pa_Initialize();
  if (err != paNoError) {
    std::cout << "Error intializing Port Audio" << std::endl;
    std::cout << Pa_GetErrorText(err);
    return 1;
  }

  return 0;
}

int PortAudioSystem::destroy() {
  PaError err = Pa_Terminate();
  if(err != paNoError) {
    std::cout << "Failed to terminate Port Audio" << std::endl;
    std::cout << Pa_GetErrorText(err);
    return 1;
  }

  return 0;
}

int PortAudioSystem::stopStream() {
  PaError err = Pa_CloseStream(stream);

  if (err) {
    std::cout << "Error closing stream" << std::endl;
    return 1;
  }

  return 0;
}

int PortAudioSystem::createStream(int bitsPerSample, int numChannels, int sampleRate) {
  PaError err;

  minReadSamples = sampleRate * 0.1;

  int ringBufferSize = 16384;
  int ringBufferStorageSize = bitsPerSample / 8 * ringBufferSize;

  ringBufferStorage = static_cast<char*>(PaUtil_AllocateMemory(ringBufferStorageSize));

  if (ringBufferStorage == NULL) {
    std::cerr << "Failed to allocate memory for ring buffer" << std::endl;
    return 1;
  }

  ring_buffer_size_t ringBufferInit = PaUtil_InitializeRingBuffer(&ringBuffer, bitsPerSample / 8, ringBufferSize, ringBufferStorage);

  if (ringBufferInit == -1) {
    std::cerr << "Failed to initialize ring buffer" << std::endl;
    return 1;
  }

  PaStreamParameters inputParameters;

  inputParameters.device = Pa_GetDefaultInputDevice();

  if (inputParameters.device == paNoDevice) {
    std::cout << "Could not get default input device" << std::endl;
    return 1;
  }

  inputParameters.channelCount = numChannels;
  inputParameters.sampleFormat = paInt16;
  inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency;
  inputParameters.hostApiSpecificStreamInfo = NULL;

  PaStreamParameters outputParameters;

  outputParameters.device = Pa_GetDefaultOutputDevice();

  if (inputParameters.device == paNoDevice) {
    std::cout << "Could not get default output device" << std::endl;
    return 1;
  }

  outputParameters.channelCount = numChannels;
  outputParameters.sampleFormat = paInt16;
  outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowInputLatency;
  outputParameters.hostApiSpecificStreamInfo = NULL;

  err = Pa_OpenStream(&stream,
                      &inputParameters,
                      NULL,
                      sampleRate,
                      paFramesPerBufferUnspecified,
                      paClipOff,
                      recordCallback,
                      this);
  if (err) {
    std::cout << "Error opening Port Audio stream" << std::endl;
    std::cout << Pa_GetErrorText(err);
    return 1;
  }

  return 0;
}

int PortAudioSystem::startStream() {
  PaError err = 0;

  err = Pa_StartStream(stream);

  if (err) {
    std::cout << "Error starting stream" << std::endl;
    return 1;
  }

  return 0;
}
