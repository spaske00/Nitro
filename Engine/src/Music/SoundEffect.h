#pragma once
#pragma once
#include <SDL_mixer.h>
namespace Engine {

	class SoundEffect {
	public:
		friend class AudioManager;

		void play(int loops = 0);
		~SoundEffect();
	private:
		Mix_Chunk* m_chunk = nullptr;
	};

};


