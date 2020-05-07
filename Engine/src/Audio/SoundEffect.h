
#pragma once


namespace Engine {

	class SoundEffect {
	public:
		friend class AudioManager;

		void play(int loops = 0);
		void Destroy();
		~SoundEffect();
	private:
		Mix_Chunk* m_chunk = nullptr;
	};

};
