#include "receiver/receiver.h"
#include <ctime>
#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>
#include <string>
#include <iostream>

int main(int argc, char **argv) {
    std::time_t result = std::time(nullptr);
    std::cout << "Receiver started at: " << std::asctime(std::localtime(&result)) << std::endl;

    //TODO pull receiver binding from command line
    receiver::JukeboxClient client(grpc::CreateChannel("localhost:9834", grpc::InsecureChannelCredentials()));
    client.ReceiverChat();
    return 0;
}
