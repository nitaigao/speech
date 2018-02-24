// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: google/cloud/dialogflow/v2beta1/tier.proto

#include "google/cloud/dialogflow/v2beta1/tier.pb.h"
#include "google/cloud/dialogflow/v2beta1/tier.grpc.pb.h"

#include <grpc++/impl/codegen/async_stream.h>
#include <grpc++/impl/codegen/async_unary_call.h>
#include <grpc++/impl/codegen/channel_interface.h>
#include <grpc++/impl/codegen/client_unary_call.h>
#include <grpc++/impl/codegen/method_handler_impl.h>
#include <grpc++/impl/codegen/rpc_service_method.h>
#include <grpc++/impl/codegen/service_type.h>
#include <grpc++/impl/codegen/sync_stream.h>
namespace google {
namespace cloud {
namespace dialogflow {
namespace v2beta1 {

static const char* Tiers_method_names[] = {
  "/google.cloud.dialogflow.v2beta1.Tiers/GetAgentTier",
  "/google.cloud.dialogflow.v2beta1.Tiers/UpdateAgentTier",
};

std::unique_ptr< Tiers::Stub> Tiers::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< Tiers::Stub> stub(new Tiers::Stub(channel));
  return stub;
}

Tiers::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel)
  : channel_(channel), rpcmethod_GetAgentTier_(Tiers_method_names[0], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_UpdateAgentTier_(Tiers_method_names[1], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status Tiers::Stub::GetAgentTier(::grpc::ClientContext* context, const ::google::cloud::dialogflow::v2beta1::GetAgentTierRequest& request, ::google::cloud::dialogflow::v2beta1::AgentTier* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_GetAgentTier_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::google::cloud::dialogflow::v2beta1::AgentTier>* Tiers::Stub::AsyncGetAgentTierRaw(::grpc::ClientContext* context, const ::google::cloud::dialogflow::v2beta1::GetAgentTierRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::google::cloud::dialogflow::v2beta1::AgentTier>::Create(channel_.get(), cq, rpcmethod_GetAgentTier_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::google::cloud::dialogflow::v2beta1::AgentTier>* Tiers::Stub::PrepareAsyncGetAgentTierRaw(::grpc::ClientContext* context, const ::google::cloud::dialogflow::v2beta1::GetAgentTierRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::google::cloud::dialogflow::v2beta1::AgentTier>::Create(channel_.get(), cq, rpcmethod_GetAgentTier_, context, request, false);
}

::grpc::Status Tiers::Stub::UpdateAgentTier(::grpc::ClientContext* context, const ::google::cloud::dialogflow::v2beta1::UpdateAgentTierRequest& request, ::google::cloud::dialogflow::v2beta1::AgentTier* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_UpdateAgentTier_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::google::cloud::dialogflow::v2beta1::AgentTier>* Tiers::Stub::AsyncUpdateAgentTierRaw(::grpc::ClientContext* context, const ::google::cloud::dialogflow::v2beta1::UpdateAgentTierRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::google::cloud::dialogflow::v2beta1::AgentTier>::Create(channel_.get(), cq, rpcmethod_UpdateAgentTier_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::google::cloud::dialogflow::v2beta1::AgentTier>* Tiers::Stub::PrepareAsyncUpdateAgentTierRaw(::grpc::ClientContext* context, const ::google::cloud::dialogflow::v2beta1::UpdateAgentTierRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::google::cloud::dialogflow::v2beta1::AgentTier>::Create(channel_.get(), cq, rpcmethod_UpdateAgentTier_, context, request, false);
}

Tiers::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      Tiers_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< Tiers::Service, ::google::cloud::dialogflow::v2beta1::GetAgentTierRequest, ::google::cloud::dialogflow::v2beta1::AgentTier>(
          std::mem_fn(&Tiers::Service::GetAgentTier), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      Tiers_method_names[1],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< Tiers::Service, ::google::cloud::dialogflow::v2beta1::UpdateAgentTierRequest, ::google::cloud::dialogflow::v2beta1::AgentTier>(
          std::mem_fn(&Tiers::Service::UpdateAgentTier), this)));
}

Tiers::Service::~Service() {
}

::grpc::Status Tiers::Service::GetAgentTier(::grpc::ServerContext* context, const ::google::cloud::dialogflow::v2beta1::GetAgentTierRequest* request, ::google::cloud::dialogflow::v2beta1::AgentTier* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status Tiers::Service::UpdateAgentTier(::grpc::ServerContext* context, const ::google::cloud::dialogflow::v2beta1::UpdateAgentTierRequest* request, ::google::cloud::dialogflow::v2beta1::AgentTier* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace google
}  // namespace cloud
}  // namespace dialogflow
}  // namespace v2beta1
