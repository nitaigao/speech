#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <time.h>

#include <grpc++/grpc++.h>

#include "portaudiosystem.h"
#include "hotword.h"

#include "google/cloud/dialogflow/v2beta1/session.grpc.pb.h"

using namespace google::cloud::dialogflow::v2beta1;
using namespace std::chrono;

class SpeechCall {

public:

  SpeechCall(grpc::CompletionQueue* cq, grpc::ClientContext* context, Sessions::Stub* service)
    : cq_(cq)
    , context_(context)
    , service_(service) {

    }

  void DetectIntent(int sampleRate, int16_t* buffer, size_t bufferSize) {
    request_.set_session("projects/baxter-dafd4/agent/sessions/123");

    auto* input = request_.mutable_query_input();

    // InputAudioConfig* audioConfig = input->mutable_audio_config();
    // audioConfig->set_language_code("en-GB");
    // audioConfig->set_sample_rate_hertz(sampleRate);
    // audioConfig->set_audio_encoding(AudioEncoding::AUDIO_ENCODING_LINEAR_16);
    // request_.set_input_audio((char*)buffer, bufferSize);

    TextInput* textInput = input->mutable_text();
    textInput->set_text("Open Chrome");
    textInput->set_language_code("en-GB");

    auto streamer = service_->AsyncDetectIntent(context_, request_, cq_);

    streamer->Finish(&response_, &status_, this);
  }

  DetectIntentResponse* response() {
    return &response_;
  }

private:

  grpc::CompletionQueue* cq_;
  grpc::ClientContext* context_;
  Sessions::Stub* service_;

  DetectIntentRequest request_;
  DetectIntentResponse response_;
  grpc::Status status_;

};

int main() {

  auto creds = grpc::GoogleDefaultCredentials();
  auto channel = grpc::CreateChannel("dialogflow.googleapis.com", creds);

  std::unique_ptr<Sessions::Stub> service(Sessions::NewStub(channel));

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

  DetectIntentRequest request;
  request.set_session("projects/baxter-dafd4/agent/sessions/123");
  // TextInput* textInput = input->mutable_text();
  // textInput->set_text("Open Chrome");
  // textInput->set_language_code("en-GB");
  // request.set_input_audio((char*)buffer.data(), buffer.size());
  // DetectIntentResponse response;
  grpc::ClientContext context;
  // grpc::Status status = service->DetectIntent(&context, request, &response);
  // std::clog << status.ok() << " " << status.error_code() << " " << status.error_message() << std::endl;
  // auto queryResult = response.query_result();
  // auto fulfillmentText = queryResult.fulfillment_text();
  // std::clog << fulfillmentText << std::endl;

  grpc::CompletionQueue cq;

  std::vector<int16_t> buffer;

  gpr_timespec deadline;

  deadline.tv_sec = 0;
  deadline.tv_nsec = 0;
  deadline.clock_type = GPR_TIMESPAN;

  while(true) {
    portAudio.read(&buffer);
    if (buffer.size() != 0) {
      int detectResult = hotword.detect(buffer.data(), buffer.size());
      if (detectResult == 1) {
        std::clog << "hotword detected" << std::endl;
        SpeechCall* speechCall = new SpeechCall(&cq, &context, service.get());
        std::clog << speechCall << std::endl;
        speechCall->DetectIntent(sampleRate, buffer.data(), buffer.size());
      }
    }
    bool ok = false;
    void* tag;
    cq.AsyncNext(&tag, &ok, deadline);
    if (ok && tag) {
      std::clog << "got response" << std::endl;
      std::clog << tag << std::endl;
      auto speechCall = reinterpret_cast<SpeechCall*>(tag);
      auto response = speechCall->response();

      auto queryResult = response->query_result();
      // auto queryText = queryResult.query_text();
      // std::clog << queryText << std::endl;
      // auto fulfillmentText = queryResult.fulfillment_text();
      // std::clog << fulfillmentText << std::endl;
    }
  }

  portAudio.stopStream();

  PortAudioSystem::destroy();
}
