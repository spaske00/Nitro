#pragma once

namespace Engine
{
	class AudioManager;
}

namespace Nitro
{
	class AudioController
	{
	public:
		bool Init(Engine::AudioManager* audioManager_);
		void Update(float dt, Engine::AudioManager* audioManager_);
		static std::unique_ptr<AudioController> Create()
		{
			return std::make_unique<AudioController>();
		}
	};
}


