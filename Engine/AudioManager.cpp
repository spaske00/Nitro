#include "precomp.h"
#include "AudioManager.h"
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
            LOG_WARNING(fmt::format("Mix_Init failed {}", Mix_GetError()));
            return false;
        }

        if (-1 == Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096)) {
            LOG_WARNING(fmt::format("Mix_Init failed! {}", Mix_GetError()));
            return false;
        }

        m_IsInitialized = true;

        return m_IsInitialized;
    }
    void AudioManager::Destroy() {
        if (m_IsInitialized) 
        {
            m_IsInitialized = false;

            for (auto it = m_MusicMap.begin(); it != m_MusicMap.end(); it++)
                (*it).second->Destroy();

            for (auto it = m_SoundEffectMap.begin(); it != m_SoundEffectMap.end(); it++)
                (*it).second->Destroy();

            Mix_Quit();
        }
    }

    bool AudioManager::LoadSoundEffect(const std::string& filePath, const std::string& name) {
        auto it = m_SoundEffectMap.find(name);
        SoundEffect effect;
        if (m_SoundEffectMap.end() == it) {
            //failed
            Mix_Chunk* chunk = Mix_LoadWAV(filePath.c_str());
            if (nullptr == chunk)
            {
                LOG_WARNING(fmt::format("Mix_LoadSoundEffect failed! ", Mix_GetError()));
                return false;
            }
            effect.m_chunk = chunk;
            m_SoundEffectMap.emplace(name, std::make_unique<SoundEffect>(effect));
        }
        return true;

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

    bool  AudioManager::LoadMusic(const std::string& filePath, const std::string& name) {
        auto it = m_MusicMap.find(name);
        Music music;
        if (m_MusicMap.end() == it) {
            //failed
            Mix_Music* mus = Mix_LoadMUS(filePath.c_str());
            if (nullptr == mus)
            {
                LOG_WARNING(fmt::format("Mix_LoadMusic failed! ", Mix_GetError()));
                return false;
            }
            music.m_music = mus;
            m_MusicMap.emplace(name, std::make_unique<Music>(music));
        }
        return true;

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