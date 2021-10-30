#include "sound/sound.h"

namespace sound {
    SDL_AudioSpec initSdlAudioSpec() {
        if (SDL_Init(SDL_INIT_AUDIO) != 0) {
            std::cout << "Unable to initialize SDL: " << SDL_GetError() << std::endl;
            exit(1);
        }

        SDL_AudioSpec spec;

        SDL_zero(spec);
        spec.freq = 44100;
        spec.format = AUDIO_S16;
        spec.channels = 2;
        spec.samples = 1152;

        return spec;
    }

    SDL_AudioDeviceID initSdlAudioDeviceId() {
        SDL_AudioDeviceID deviceId = SDL_OpenAudioDevice(NULL, 0, &initSdlAudioSpec(), NULL, 0);
        SDL_PauseAudioDevice(deviceId, 0);
        return deviceId;
    }

    Player::Player() : sdlAudioDeviceId(initSdlAudioDeviceId()) {}

    void Player::queue(jukebox::PlaySongChunk chunk) {
        Uint32 length = chunk.size();
        Uint8 *buffer = (uint8_t *) chunk.chunk().c_str();

        int success = SDL_QueueAudio(sdlAudioDeviceId, buffer, length);
        if (success != 0) {
            std::cout << "Failed to queue audio!" << std::endl;
        }
        return;
    }
}