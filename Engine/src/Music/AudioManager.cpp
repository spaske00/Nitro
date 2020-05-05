#include "precomp.h"
#include "AudioManager.h"
#include "Logger/Logger.h"
#include "SoundEffect.h"
#include "Music.h"
namespace Engine {

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

            for (auto it = m_MusicMap.begin(); it != m_MusicMap.end(); it++)
                (*it).second->Destroy();

            for (auto it = m_SoundEffectMap.begin(); it != m_SoundEffectMap.end(); it++)
                (*it).second->Destroy();

            Mix_Quit();
        }
    }

    void AudioManager::LoadSoundEffect(const std::string& filePath, const std::string& name) {
        auto it = m_SoundEffectMap.find(name);
        SoundEffect effect;
        if (m_SoundEffectMap.end() == it) {
            //failed
            Mix_Chunk* chunk = Mix_LoadWAV(filePath.c_str());
            if (nullptr == chunk)
            {
                LOG_WARNING("Mix_LoadSoundEffect failed! " + std::string(Mix_GetError()));

            }
            effect.m_chunk = chunk;
            m_SoundEffectMap.emplace(name, std::make_unique<SoundEffect>(effect));
        }

    }

    void AudioManager::PlaySoundEffect(const std::string& name) {
        auto it = m_SoundEffectMap.find(name);
        if (m_SoundEffectMap.end() == it) {
            LOG_WARNING("SoundEffect wasn't loaded");
        }
        else {

            it->second->play(1);
        }
    }

    void  AudioManager::LoadMusic(const std::string& filePath, const std::string& name) {
        auto it = m_MusicMap.find(name);
        Music music;
        if (m_MusicMap.end() == it) {
            //failed
            Mix_Music* mus = Mix_LoadMUS(filePath.c_str());
            if (nullptr == mus)
            {
                LOG_WARNING("Mix_LoadMusic failed! " + std::string(Mix_GetError()));
            }
            music.m_music = mus;
            m_MusicMap.emplace(name, std::make_unique<Music>(music));

        }

    }
    void AudioManager::PlayMusic(const std::string& name) {
        auto it = m_MusicMap.find(name);
        if (m_MusicMap.end() == it) {
            LOG_WARNING("Music wasn't loaded");
        }
        else {

            it->second->play();
        }
    }

};