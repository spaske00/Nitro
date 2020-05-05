
#pragma once

namespace Engine
{
	class Music {
	public:
		friend class AudioManager;

		void play();
		static void pause();
		static void stop();
		static void resume();
		void Destroy();
	private:
		Mix_Music* m_music = nullptr;
	};

};