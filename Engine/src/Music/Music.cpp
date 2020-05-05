#include "precomp.h"
#include "Music.h"

namespace Engine {
    void Music::play() {

        if (-1 == Mix_PlayMusic(m_music, -1)) {
            LOG_WARNING("Play Music failed! " + std::string(Mix_GetError()));

        }
    }

    void Music::pause() {

        Mix_PausedMusic();

    }

    void Music::stop() {

        Mix_HaltMusic();
    }

    void Music::resume() {

        Mix_ResumeMusic();

    }

    void Music::Destroy() {
        if (nullptr != m_music)
            Mix_FreeMusic(m_music);
    }


};