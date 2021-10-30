#include "receiver.h"
#include "proto/jukebox.grpc.pb.h"
#include <ctime>
#include <iostream>
#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

jukebox::PlaySongHeaderResponse MakePlaySongHeaderResponse() {
    jukebox::PlaySongHeaderResponse r;
    return r;
}

jukebox::PlaySongChunkResponse MakePlaySongChunkResponse() {
    jukebox::PlaySongChunkResponse r;
    return r;
}

jukebox::PlaySongTrailerResponse MakePlaySongTrailerResponse() {
    jukebox::PlaySongTrailerResponse r;
    return r;
}

jukebox::PlaySongHeaderResponse handlePlaySongHeader(jukebox::ReceiverCommandRequest cmd) {
    switch (cmd.command_case()) {
        case jukebox::ReceiverCommandRequest::CommandCase::kPlaySongHeader:
            std::cout << "Playing song " << cmd.playsongheader().song().SerializeAsString() << " ..."
                      << std::endl;
            return MakePlaySongHeaderResponse();
        case jukebox::ReceiverCommandRequest::CommandCase::kPlaySongChunk:
        case jukebox::ReceiverCommandRequest::CommandCase::kPlaySongTrailer:
        default:
            std::cout << "Expected a PlaySongHeader but didn't get one, bailing..." << std::endl;
            throw std::invalid_argument("Unexpected message type while awaiting PlaySongHeader");
    }
}

jukebox::PlaySongChunkResponse handlePlaySongChunk(jukebox::ReceiverCommandRequest cmd) {
    switch (cmd.command_case()) {
        case jukebox::ReceiverCommandRequest::CommandCase::kPlaySongChunk:
            return MakePlaySongChunkResponse();
        case jukebox::ReceiverCommandRequest::CommandCase::kPlaySongHeader:
        case jukebox::ReceiverCommandRequest::CommandCase::kPlaySongTrailer:
        default:
            std::cout << "Expected a PlaySongChunk but didn't get one, bailing..." << std::endl;
            throw std::invalid_argument("Unexpected message type while awaiting PlaySongChunk");
    }
}

jukebox::PlaySongTrailerResponse handlePlaySongTrailer(jukebox::ReceiverCommandRequest cmd) {
    switch (cmd.command_case()) {
        case jukebox::ReceiverCommandRequest::CommandCase::kPlaySongTrailer:
            std::cout << "Finished receiving song chunks!" << std::endl;
            return MakePlaySongTrailerResponse();
        case jukebox::ReceiverCommandRequest::CommandCase::kPlaySongHeader:
        case jukebox::ReceiverCommandRequest::CommandCase::kPlaySongChunk:
        default:
            std::cout << "Expected a PlaySongTrailer but didn't get one, bailing..." << std::endl;
            throw std::invalid_argument("Unexpected message type while awaiting PlaySongTrailer");
    }
}

namespace receiver {
    JukeboxClient::JukeboxClient(std::shared_ptr <grpc::Channel> channel) : stub_(
            jukebox::ReceiverService::NewStub(channel)) {}

    void JukeboxClient::ReceiverChat() {
        grpc::ClientContext context;
        std::shared_ptr <grpc::ClientReaderWriter<jukebox::ReceiverCommandResponse, jukebox::ReceiverCommandRequest>> stream(
                stub_->ReceiverChat(&context)
        );

        jukebox::ReceiverCommandRequest receiverCommand;
        jukebox::ReceiverCommandResponse receiverResponse;

        for (;;) {
            //Play Song Header
            if (!stream->Read(&receiverCommand)) {
                std::cout << "Server is no longer sending commands, bailing..." << std::endl;
                return;
            }
            receiverResponse.Clear();
            receiverResponse.mutable_playsongheaderresponse()->CopyFrom(
                    handlePlaySongHeader(receiverCommand));
            if (!stream->Write(receiverResponse)) {
                std::cout << "Failed to send play song header response, bailing..." << std::endl;
            }

            std::cout << "Receiving song chunks..." << std::endl;

            bool trailerReceived = false;
            while (!trailerReceived) {
                if (!stream->Read(&receiverCommand)) {
                    std::cout << "Server is no longer sending commands, bailing..." << std::endl;
                    return;
                }

                switch (receiverCommand.command_case()) {
                    case jukebox::ReceiverCommandRequest::CommandCase::kPlaySongChunk:
                        //TODO Play Song Chunk
                        break;
                    case jukebox::ReceiverCommandRequest::CommandCase::kPlaySongTrailer:
                        //Play Song Trailer
                        receiverResponse.Clear();
                        receiverResponse.mutable_playsongtrailerresponse()->CopyFrom(
                                handlePlaySongTrailer(receiverCommand));
                        if (!stream->Write(receiverResponse)) {
                            std::cout << "Failed to send play song chunk response, bailing..." << std::endl;
                        }
                        trailerReceived = true;
                        break;
                    default:
                        std::cout
                                << "Unexpected receiver command received while reading song chunks and awaiting trailer, bailing..."
                                << std::endl;
                        return;
                }
            }
        }

        receiverCommand.Clear();
        receiverResponse.Clear();
    };
}