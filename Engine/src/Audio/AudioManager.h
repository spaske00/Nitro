
#pragma once

namespace Engine 
{
	class AudioManager
	{
	public:
		~AudioManager();

		bool Init();

		bool LoadSoundEffect(const std::string& filePath, const std::string& name);
		void PlaySoundEffect(const std::string& name);
		bool LoadMusic(const std::string& filePath, const std::string& name);
		void PlayMusic(const std::string& name);

	private:
		std::map<std::string, Mix_Chunk*> m_SoundEffectMap;
		std::map<std::string, Mix_Music*> m_MusicMap;
		bool m_IsInitialized = false;	
	};

}
