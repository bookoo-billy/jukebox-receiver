//
// Created by Will on 10/30/2021.
//

#ifndef JUKEBOX_RECEIVER_SOUND_H
#define JUKEBOX_RECEIVER_SOUND_H

#include "proto/jukebox.grpc.pb.h"
#include "SDL.h"
#include <queue>

namespace sound {
    class Player {
    public:
        Player();

        void queue(jukebox::PlaySongChunk);
        void Player::playCallback(void* userdata, Uint8* stream, int len);

    private:
        SDL_AudioDeviceID sdlAudioDeviceId;
        std::queue<jukebox::PlaySongChunk> chunkQueue;

        SDL_AudioDeviceID Player::initSdlAudioDeviceId();
        SDL_AudioSpec Player::initSdlAudioSpec();
    };
}

#endif //JUKEBOX_RECEIVER_SOUND_H
