#pragma once
#pragma once
#include <SDL_mixer.h>
#include <string>
#include <map>
namespace Engine {

	class SoundEffect;
	class Music;

	class AudioManager
	{
	public:
		AudioManager();
		~AudioManager();

		bool Init();
		void Destroy();

		void LoadSoundEffect(const std::string& filePath, const std::string& name);
		void PlaySoundEffect(const std::string& name);
		void LoadMusic(const std::string& filePath, const std::string& name);
		void PlayMusic(const std::string& name);

	private:
		std::map<std::string, std::unique_ptr<SoundEffect>> m_SoundEffectMap;
		std::map<std::string, std::unique_ptr<Music>> m_MusicMap;
		bool m_IsInitialized = false;
	};

}

