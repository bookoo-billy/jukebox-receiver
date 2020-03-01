# CMakeList.txt : CMake project for jukebox-receiver, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

find_package(Boost 1.65 REQUIRED COMPONENTS system)
find_package(Protobuf 3.6 REQUIRED)

find_path(PORTAUDIO_INCLUDE_DIR portaudio.h)
find_library(PORTAUDIO_LIBRARY portaudio)

include_directories(${Boost_INCLUDE_DIRS} ${Protobuf_INCLUDE_DIRS} ${PORTAUDIO_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

PROTOBUF_GENERATE_CPP(JUKEBOX_PROTO_SRCS JUKEBOX_PROTO_HDRS jukebox.proto)

# Add source to this project's executable.
add_executable (jukebox-receiver "jukebox-receiver.cpp" "jukebox-receiver.h" ${JUKEBOX_PROTO_SRCS} ${JUKEBOX_PROTO_HDRS})

# TODO: Add tests and install targets if needed.
target_link_libraries(jukebox-receiver ${Boost_LIBRARIES} ${Protobuf_LIBRARIES} ${PORTAUDIO_LIBRARY})
