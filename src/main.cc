#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <queue>
#include <time.h>

#include <grpc++/grpc++.h>

#include "portaudiosystem.h"
#include "hotword.h"
#include "snowboy-vad-cxx-compat.h"

#include <sndfile.hh>

#include "google/cloud/dialogflow/v2beta1/session.grpc.pb.h"

using namespace google::cloud::dialogflow::v2beta1;
using namespace std::chrono;

class SpeechCall {

public:

  SpeechCall(grpc::CompletionQueue* cq, Sessions::Stub* service)
    : cq_(cq)
    , service_(service) {

    }

  void DetectIntent(int sampleRate, const std::vector<int16_t>& buffer) {
    audio_.insert(audio_.end(), buffer.begin(), buffer.end());
    request_.set_session("projects/baxter-dafd4/agent/sessions/123");

    auto* input = request_.mutable_query_input();

    InputAudioConfig* audioConfig = input->mutable_audio_config();
    audioConfig->set_language_code("en-GB");
    audioConfig->set_sample_rate_hertz(sampleRate);
    audioConfig->set_audio_encoding(AudioEncoding::AUDIO_ENCODING_LINEAR_16);
    request_.set_input_audio((char*)audio_.data(), audio_.size() * sizeof(int16_t));

    // TextInput* textInput = input->mutable_text();
    // textInput->set_text("Seattle");
    // textInput->set_language_code("en-GB");

    auto streamer = service_->AsyncDetectIntent(&context_, request_, cq_);

    streamer->Finish(&response_, &status_, this);
  }

  DetectIntentResponse* response() {
    return &response_;
  }

private:

  grpc::CompletionQueue* cq_;
  grpc::ClientContext context_;
  Sessions::Stub* service_;

  DetectIntentRequest request_;
  DetectIntentResponse response_;
  grpc::Status status_;
  std::vector<int16_t> audio_;

};

int main() {

  auto creds = grpc::GoogleDefaultCredentials();
  auto channel = grpc::CreateChannel("dialogflow.googleapis.com", creds);

  std::unique_ptr<Sessions::Stub> service(Sessions::NewStub(channel));

  PortAudioSystem::init();

  std::string modelFileName = "./resources/seattle.pmdl";
  Hotword hotword(modelFileName);
  hotword.init("0.6", 2.0f);

  int bitsPerSample = hotword.bitsPerSample();
  int numChannels = hotword.numChannels();
  int sampleRate = hotword.sampleRate();

  SnowboyVad vad("./resources/common.res");
  vad.SetAudioGain(1.0f);

  PortAudioSystem portAudio;

  portAudio.createStream(
    bitsPerSample,
    numChannels,
    sampleRate
  );

  portAudio.startStream();

  DetectIntentRequest request;
  request.set_session("projects/baxter-dafd4/agent/sessions/123");

  grpc::CompletionQueue cq;

  std::vector<int16_t> buffer;

  gpr_timespec deadline;

  deadline.tv_sec = 0;
  deadline.tv_nsec = 0;
  deadline.clock_type = GPR_TIMESPAN;

  std::queue<std::vector<int16_t>> speechBuffer;
  speechBuffer.push(std::vector<int16_t>(sampleRate, 0));

  std::queue<std::vector<int16_t>> commandBuffer;
  speechBuffer.push(std::vector<int16_t>(sampleRate, 0));

  bool capturingCommand = false;

  while(true) {
    portAudio.read(&buffer);
    if (buffer.size() != 0) {
      int vadResult = vad.RunVad(buffer.data(), buffer.size());

      if (vadResult == 0) {
        speechBuffer.push(buffer);
      } else {
        std::queue<std::vector<int16_t>> empty;
        empty.push(std::vector<int16_t>(sampleRate, 0));
        std::swap(speechBuffer, empty);
      }

      if (capturingCommand && vadResult != 0) {
        capturingCommand = false;
        std::clog << commandBuffer.size() << std::endl;

        std::clog << "sending command" << std::endl;
        SpeechCall* speechCall = new SpeechCall(&cq, service.get());

        std::vector<int16_t> fullCommandBuffer;
        commandBuffer.push(std::vector<int16_t>(sampleRate, 0));

        std::clog << "combining " << commandBuffer.size() << std::endl;

        while (!commandBuffer.empty()) {
          auto sample = commandBuffer.front();
          fullCommandBuffer.insert(fullCommandBuffer.end(), sample.begin(), sample.end());
          commandBuffer.pop();
        }

        speechCall->DetectIntent(sampleRate, fullCommandBuffer);

        SndfileHandle outfile("/tmp/command.wav", SFM_WRITE, SF_FORMAT_WAV | SF_FORMAT_PCM_16, numChannels, sampleRate);
        outfile.write(fullCommandBuffer.data(), fullCommandBuffer.size());
      }

      if (capturingCommand && vadResult == 0) {
        std::clog << "writing command" << std::endl;
        commandBuffer.push(buffer);
        std::clog << commandBuffer.size() << std::endl;
      } else {
        std::queue<std::vector<int16_t>> empty;
        empty.push(std::vector<int16_t>(sampleRate, 0));
        std::swap(commandBuffer, empty);
      }

      // std::clog << vadResult << std::endl;

      int detectResult = hotword.detect(buffer.data(), buffer.size());
      if (detectResult == 1) {
        capturingCommand = true;
        std::clog << "hotword detected" << std::endl;
        SpeechCall* speechCall = new SpeechCall(&cq, service.get());

        std::vector<int16_t> fullSpeechBuffer;
        speechBuffer.push(std::vector<int16_t>(sampleRate, 0));

        std::clog << "combining " << speechBuffer.size() << std::endl;

        while (!speechBuffer.empty()) {
          auto sample = speechBuffer.front();
          fullSpeechBuffer.insert(fullSpeechBuffer.end(), sample.begin(), sample.end());
          speechBuffer.pop();
        }

        speechCall->DetectIntent(sampleRate, fullSpeechBuffer);

        SndfileHandle outfile("/tmp/buffer.wav", SFM_WRITE, SF_FORMAT_WAV | SF_FORMAT_PCM_16, numChannels, sampleRate);
        outfile.write(fullSpeechBuffer.data(), fullSpeechBuffer.size());
      }
    }
    bool ok = false;
    void* tag;
    auto result = cq.AsyncNext(&tag, &ok, deadline);
    bool gotEvent = result == grpc::CompletionQueue::GOT_EVENT;
    if (ok && gotEvent && tag) {
      std::clog << "got response" << std::endl;
      auto speechCall = reinterpret_cast<SpeechCall*>(tag);
      auto response = speechCall->response();

      auto queryResult = response->query_result();
      auto queryText = queryResult.query_text();
      std::clog << "Query: " << queryText << std::endl;
      auto fulfillmentText = queryResult.fulfillment_text();
      std::clog << "Fulfillment: " << fulfillmentText << std::endl;
      auto languageCode = queryResult.language_code();
      std::clog << "Language Code: " << languageCode << std::endl;
      auto intent = queryResult.intent();
      auto intentName = intent.name();
      std::clog << "Intent: " << intentName << std::endl;
      delete speechCall;

    }
  }

  portAudio.stopStream();

  PortAudioSystem::destroy();
}
