#ifndef JUKEBOX_RECEIVER_H
#define JUKEBOX_RECEIVER_H

#include "proto/jukebox.grpc.pb.h"
#include <grpcpp/channel.h>

namespace receiver {
    class JukeboxClient {
    public:
        JukeboxClient(std::shared_ptr <grpc::Channel> channel);
        void ReceiverChat();

    private:
        std::unique_ptr <jukebox::ReceiverService::Stub> stub_;
    };
}
#endif //JUKEBOX_RECEIVER_H
