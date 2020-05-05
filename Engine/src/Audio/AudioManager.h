
#pragma once


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

		bool LoadSoundEffect(const std::string& filePath, const std::string& name);
		void PlaySoundEffect(const std::string& name);
		bool LoadMusic(const std::string& filePath, const std::string& name);
		void PlayMusic(const std::string& name);

	private:
		std::map<std::string, std::unique_ptr<SoundEffect>> m_SoundEffectMap;
		std::map<std::string, std::unique_ptr<Music>> m_MusicMap;
		bool m_IsInitialized = false;
	};

}
