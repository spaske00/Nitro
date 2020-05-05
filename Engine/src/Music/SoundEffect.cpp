#pragma once
#include "precomp.h"
#include "SoundEffect.h"

namespace Engine {
    void SoundEffect::play(int loops) {
        if (-1 == Mix_PlayChannel(-1, m_chunk, loops)) {
            LOG_WARNING("Play SoundEffect failed! " + std::string(Mix_GetError()));
        }
    }

    void SoundEffect::Destroy() {
        if (nullptr != m_chunk)
            Mix_FreeChunk(m_chunk);
    }
};