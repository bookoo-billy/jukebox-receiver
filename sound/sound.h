//
// Created by Will on 10/30/2021.
//

#ifndef JUKEBOX_RECEIVER_SOUND_H
#define JUKEBOX_RECEIVER_SOUND_H

#include "proto/jukebox.grpc.pb.h"
#include "SDL.h"
namespace sound {
    class Player {
    public:
        Player();

        void queue(jukebox::PlaySongChunk);

    private:
        SDL_AudioDeviceID sdlAudioDeviceId;
    };
}

#endif //JUKEBOX_RECEIVER_SOUND_H
