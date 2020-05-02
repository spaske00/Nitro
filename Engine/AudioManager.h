#pragma once
#include <SDL_mixer.h>
#include <string>
#include <map>
namespace Engine {

	class SoundEffect {
	public:
		friend class AudioManager;
		void play(int loops = 0);
	private:
		Mix_Chunk* m_chunk = nullptr;
	};

	class Music {
	public:
		friend class AudioManager;

		void play();
		static void pause();
		static void stop();
		static void resume();

	private:
		Mix_Music* m_music = nullptr;
	};

	class AudioManager
	{
	public:
		AudioManager();
		~AudioManager();

		bool Init();
		void Destroy();

		void LoadSoundEffect(const std::string& filePath);
		void playSoundEffect(const std::string& filePath);
		Music LoadMusic(const std::string& filePath);

	private:
		std::map<std::string, SoundEffect> m_SoundEffectMap;
		std::map<std::string, Mix_Music*> m_MusicMap;
		bool m_IsInitialized = false;
	};

}

