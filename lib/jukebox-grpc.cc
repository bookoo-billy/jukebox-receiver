#include "lib/jukebox-grpc.h"
#include "proto/jukebox.grpc.pb.h"
#include <ctime>
#include <iostream>

void print_localtime() {
  std::time_t result = std::time(nullptr);
  std::cout << std::asctime(std::localtime(&result));
}