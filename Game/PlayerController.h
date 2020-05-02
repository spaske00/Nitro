#pragma once
namespace Engine {
	class TextureManager;
	class EntityManager;
	class AudioManager;
}

namespace Nitro
{

	class PlayerController
	{
	public:
		bool Init(Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_, Engine::AudioManager* audioManager_);
		void Update(float dt_, Engine::EntityManager* entityManager_, Engine::AudioManager* audioManager_);
		static std::unique_ptr<PlayerController> Create()
		{
			return std::make_unique<PlayerController>();
		}
	};
}
