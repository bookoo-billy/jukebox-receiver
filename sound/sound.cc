#include "sound/sound.h"
#include <google/protobuf/util/time_util.h>
#include <queue>
#include <chrono>
#include <thread>

namespace sound {
    Player::Player(SDL_AudioFormat format, int sampleRate, Uint8 channels, Uint16 samples) :
        format(format), sampleRate(sampleRate), channels(channels), samples(samples), chunkQueue(), sdlAudioDeviceId(initSdlAudioDeviceId()) {}

    //Call this when a PlaySongChunk is received
    void Player::queue(jukebox::PlaySongChunk chunk) {
        chunkQueue.push(chunk);
        return;
    }

    static void staticPlayCallback(void* userdata, Uint8* stream, int len) {
        const auto player = reinterpret_cast<sound::Player*>(userdata);
        player->playCallback(userdata, stream, len);
        return;
    }

    //This function will dequeue a PlaySongChunk and send it to SDL for playback
    void Player::playCallback(void* userdata, Uint8* stream, int len) {
        //TODO: Get this time in nanos or millis
        google::protobuf::Timestamp sysTime = google::protobuf::util::TimeUtil::GetCurrentTime();

        while (!chunkQueue.empty()) {
            jukebox::PlaySongChunk chunk = chunkQueue.front();
            chunkQueue.pop();

            google::protobuf::Timestamp playTime = chunk.timestamp();

            int playTimeSecs = playTime.seconds();
            int sysTimeSecs = sysTime.seconds();

            if (sysTimeSecs - 1 <= playTimeSecs && playTimeSecs <= sysTimeSecs + 1) {
                Uint8 *buffer = (uint8_t *) chunk.chunk().c_str();
                SDL_memcpy(stream, buffer, chunk.size());
                chunk.Clear();
                return;
            } else if (sysTimeSecs - 1 <= playTimeSecs) {
                SDL_memset(stream, 0, chunk.size());
                chunk.Clear();
                return;
            }

            chunk.Clear();
        }

        //There are no chunks available for playing, so play silence
        SDL_memset(stream, 0, 4608);
        return;
    }

    //Call this when the PlaySongChunkHeader is received
    SDL_AudioSpec Player::initSdlAudioSpec() {
        if (SDL_Init(SDL_INIT_AUDIO) != 0) {
            std::cout << "Unable to initialize SDL: " << SDL_GetError() << std::endl;
            exit(1);
        }

        SDL_AudioSpec spec;

        SDL_zero(spec);
        spec.freq = this->sampleRate;
        spec.format = this->format;
        spec.channels = this->channels;
        spec.samples = this->samples;
        spec.callback = staticPlayCallback;
        spec.userdata = this;
        return spec;
    }

    SDL_AudioDeviceID Player::initSdlAudioDeviceId() {
        SDL_AudioSpec spec = initSdlAudioSpec();
        SDL_AudioDeviceID deviceId = SDL_OpenAudioDevice(NULL, 0, &spec, NULL, 0);
        SDL_PauseAudioDevice(deviceId, 0);
        return deviceId;
    }
}