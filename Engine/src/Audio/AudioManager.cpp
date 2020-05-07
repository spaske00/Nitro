
#include "precomp.h"
#include "AudioManager.h"

namespace Engine 
{
	Mix_Chunk* SoundEffectLoad(const char* path)
	{
		Mix_Chunk* result = Mix_LoadWAV(path);
		if (!result)
		{
			LOG_WARNING(fmt::format("Mix_LoadSoundEffect failed! ", Mix_GetError()));
			return nullptr;
		}
		return result;
	}

	void SoundEffectPlay(Mix_Chunk* chunk, int loops = 0)
	{
		if (-1 == Mix_PlayChannel(-1, chunk, loops))
		{
			LOG_WARNING(fmt::format("Play {} failed! ", Mix_GetError()));
		}
	}


	void SoundEffectFree(Mix_Chunk* chunk)
	{
		if (nullptr != chunk)
		{
			Mix_FreeChunk(chunk);
		}
	}


	Mix_Music* MusicLoad(const char* path)
	{
		Mix_Music* result = Mix_LoadMUS(path);
		if (!result)
		{
			LOG_WARNING(fmt::format("Mix_LoadMusic failed! ", Mix_GetError()));
			return nullptr;
		}
		return result;
	}

	void MusicPlay(Mix_Music *music, int loops = -1)
	{
		if (-1 == Mix_PlayMusic(music, loops))
		{
			LOG_WARNING("Play Music failed! " + std::string(Mix_GetError()));
		}
	}

	void MusicFree(Mix_Music* music)
	{
		if (music)
		{
			Mix_FreeMusic(music);
		}
	}


	bool AudioManager::Init() 
	{
		// Parameters are format of music
		if (-1 == Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG)) 
		{
			LOG_WARNING(fmt::format("Mix_Init failed {}", Mix_GetError()));
			return false;
		}

		if (-1 == Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096))
		{
			LOG_WARNING(fmt::format("Mix_Init failed! {}", Mix_GetError()));
			return false;
		}

		m_IsInitialized = true;

		return m_IsInitialized;
	}

	AudioManager::~AudioManager() 
	{
		if (m_IsInitialized)
		{
			for (auto& entry : m_MusicMap)
			{
				MusicFree(entry.second);
			}

			for (auto& entry : m_SoundEffectMap)
			{
				SoundEffectFree(entry.second);
			}
			Mix_Quit();
		}
	}

	bool AudioManager::LoadSoundEffect(const std::string& filePath, const std::string& name) 
	{
		ASSERT(m_IsInitialized, "Can't use unitialized AudioManager");
		
		auto it = m_SoundEffectMap.find(name);
		if (m_SoundEffectMap.end() == it) 
		{
			if (Mix_Chunk* chunk = SoundEffectLoad(filePath.c_str()))
			{
				m_SoundEffectMap.emplace(name, chunk);
			}
			else
			{
				LOG_INFO(fmt::format("AudioManager failed to LoadSoundEffect at path {}", filePath));
			}
		}
		return true;

	}

	void AudioManager::PlaySoundEffect(const std::string& name) 
	{
		ASSERT(m_IsInitialized, "Can't use unitialized AudioManager");

		auto it = m_SoundEffectMap.find(name);
		if (m_SoundEffectMap.end() != it) 
		{
			SoundEffectPlay(it->second);
		}
		else 
		{
			LOG_WARNING("SoundEffectHandle wasn't loaded");
		}
	}

	bool AudioManager::LoadMusic(const std::string& filePath, const std::string& name)
	{
		ASSERT(m_IsInitialized, "Can't use unitialized AudioManager");

		auto it = m_MusicMap.find(name);
		if (m_MusicMap.end() == it)
		{
			if (Mix_Music* music = MusicLoad(filePath.c_str()))
			{
				m_MusicMap.emplace(name, music);
			}
			else
			{
				LOG_INFO(fmt::format("AudioManager failed to LoadMusic at path {}", filePath));
			}
		}
		return true;
	}

	void AudioManager::PlayMusic(const std::string& name)
	{
		ASSERT(m_IsInitialized, "Can't use unitialized AudioManager");
		auto it = m_MusicMap.find(name);
		if (m_MusicMap.end() != it)
		{
			MusicPlay(it->second);
		}
		else
		{
			LOG_WARNING(fmt::format("Music with name {} wasn't loaded", name));
		}
	}
};
