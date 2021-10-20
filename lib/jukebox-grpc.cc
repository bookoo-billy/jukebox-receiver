#include "lib/jukebox-grpc.h"
#include "proto/jukebox.grpc.pb.h"
#include <ctime>
#include <iostream>
#include <grpc/grpc.h>
#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

jukebox::PlaySongChunkResponse MakePlaySongChunkResponse() {
  jukebox::PlaySongChunkResponse r;
  return r;
}

class JukeboxClient {
  public:
    JukeboxClient(std::shared_ptr<grpc::Channel> channel) : stub_(jukebox::ReceiverService::NewStub(channel)) {}

    void ReceiverChat() {
      grpc::ClientContext context;
      std::shared_ptr<grpc::ClientReaderWriter<jukebox::ReceiverCommandResponse, jukebox::ReceiverCommandRequest>> stream(
        stub_->ReceiverChat(&context)
      );

      jukebox::ReceiverCommandRequest receiverCommand;
      jukebox::ReceiverCommandResponse receiverResponse;

      for(;;) {
        if (!stream->Read(&receiverCommand)) {
          std::cout << "Server is no longer sending commands, shutting down..." << std::endl;
          return;
        }

        std::cout << "Received command: " << receiverCommand.SerializeAsString() << std::endl;
        receiverResponse.mutable_playsongchunkresponse()->CopyFrom(MakePlaySongChunkResponse());

        if (!stream->Write(receiverResponse)) {
          std::cout << "Server closed stream before we could write a response" <<  std::endl;
          return;
        }

        receiverCommand.Clear();
        receiverResponse.Clear();
      }
    };
  private:
    std::unique_ptr<jukebox::ReceiverService::Stub> stub_;
};

void print_localtime() {
  std::time_t result = std::time(nullptr);
  std::cout << std::asctime(std::localtime(&result));

  JukeboxClient client(grpc::CreateChannel("localhost:9834", grpc::InsecureChannelCredentials()));

  client.ReceiverChat();
};