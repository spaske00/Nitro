#include "precomp.h"
#include "AudioManager.h"
#include "Logger/Logger.h"
#include <map>
namespace Engine {

    void SoundEffect::play(int loops) {
        if (-1 == Mix_PlayChannel(-1, m_chunk, loops)) {
            LOG_WARNING("Play SoundEffect failed! " + std::string(Mix_GetError()));
        }
    }

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

    AudioManager::AudioManager() {

    }

    AudioManager::~AudioManager() {
        Destroy();
    }

    bool AudioManager::Init() {
        // Parameters are format of music
        if (-1 == Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG)) {
            LOG_WARNING("Mix_Init failed! " + std::string(Mix_GetError()));
        }

        if (-1 == Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096)) {
            LOG_WARNING("Mix_Init failed! " + std::string(Mix_GetError()));
        }

        m_IsInitialized = true;

        return m_IsInitialized;
    }
    void AudioManager::Destroy() {
        if (m_IsInitialized) {
            m_IsInitialized = false;
            Mix_Quit();
        }
    }

    void AudioManager::LoadSoundEffect(const std::string& filePath) {
        auto it = m_SoundEffectMap.find(filePath);
        SoundEffect effect;
        if (m_SoundEffectMap.end() == it) {
            //failed
            Mix_Chunk* chunk = Mix_LoadWAV(filePath.c_str());
            if (nullptr == chunk)
            {
                LOG_WARNING("Mix_LoadSoundEffect failed! " + std::string(Mix_GetError()));

            }
            effect.m_chunk = chunk;
            m_SoundEffectMap[filePath] = effect;
        }
        
    }

    void AudioManager::playSoundEffect(const std::string& filePath) {
        auto it = m_SoundEffectMap.find(filePath);
        if (m_SoundEffectMap.end() == it) {
            LOG_WARNING("SoundEffect wasn't loaded");
        }
        it->second.play(1);
    }
    Music AudioManager::LoadMusic(const std::string& filePath) {
        auto it = m_MusicMap.find(filePath);
        Music music;
        if (m_MusicMap.end() == it) {
            //failed
            Mix_Music* mus = Mix_LoadMUS(filePath.c_str());
            if (nullptr == mus)
            {
                LOG_WARNING("Mix_LoadMusic failed! " + std::string(Mix_GetError()));
            }
            m_MusicMap[filePath] = mus;
            music.m_music = mus;

        }
        else {
            music.m_music = it->second;
        }
        return music;

    }

};