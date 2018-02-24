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
  auto* input = request.mutable_query_input();
  // InputAudioConfig* audioConfig = input->mutable_audio_config();
  // audioConfig->set_language_code("en-GB");
  // audioConfig->set_sample_rate_hertz(sampleRate);
  // audioConfig->set_audio_encoding(AudioEncoding::AUDIO_ENCODING_LINEAR_16);
  TextInput* textInput = input->mutable_text();
  textInput->set_text("Open Chrome");
  textInput->set_language_code("en-GB");
  // request.set_input_audio((char*)buffer.data(), buffer.size());
  // DetectIntentResponse response;
  grpc::ClientContext context;
  // grpc::Status status = service->DetectIntent(&context, request, &response);
  // std::clog << status.ok() << " " << status.error_code() << " " << status.error_message() << std::endl;
  // auto queryResult = response.query_result();
  // auto fulfillmentText = queryResult.fulfillment_text();
  // std::clog << fulfillmentText << std::endl;

  grpc::CompletionQueue cq;
  auto streamer = service->AsyncDetectIntent(&context, request, &cq);
  grpc::Status status;

  DetectIntentResponse response;
  streamer->Finish(&response, &status, (void*)1);

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

      }
    }
    bool ok = false;
    // Tag* tag = nullptr;
    void* got_tag;
    cq.AsyncNext(&got_tag, &ok, deadline);
    if (ok && got_tag == (void*)1) {
      auto queryResult = response.query_result();
      auto fulfillmentText = queryResult.fulfillment_text();
      std::clog << fulfillmentText << std::endl;
      // std::cout << tag->name << " completed." << std::endl;
      // tag->happening_now = false;
      // // if (tag != &create_stream) {
      // //   std::cerr << "Expected create_stream in cq." << std::endl;
      // //   return -1;
      // // }
      // if (!ok) {
      //   std::cerr << "Stream closed while creating it." << std::endl;
      //   return -1;
      // }
    }
  }

  portAudio.stopStream();

  PortAudioSystem::destroy();
}
