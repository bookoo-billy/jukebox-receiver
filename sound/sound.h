#ifndef JUKEBOX_RECEIVER_SOUND_H
#define JUKEBOX_RECEIVER_SOUND_H

#include "proto/jukebox.grpc.pb.h"
#include "SDL.h"
#include <queue>

namespace sound {
    class Player {
    public:
        Player(SDL_AudioFormat format, int sampleRate, Uint8 channels, Uint16 samples);
        void queue(jukebox::PlaySongChunk);
        void playCallback(void* userdata, Uint8* stream, int len);

    private:
        SDL_AudioFormat format;
        int sampleRate;
        Uint8 channels;
        Uint16 samples;
        std::queue<jukebox::PlaySongChunk> chunkQueue;
        SDL_AudioDeviceID sdlAudioDeviceId;

        SDL_AudioDeviceID initSdlAudioDeviceId();
        SDL_AudioSpec initSdlAudioSpec();
    };
}

#endif //JUKEBOX_RECEIVER_SOUND_H
