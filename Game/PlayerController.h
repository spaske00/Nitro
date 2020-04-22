#pragma once
namespace Engine {
	class TextureManager;
	class EntityManager;
}

namespace Nitro
{

	class PlayerController
	{
	public:
		bool Init(Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_);
		void Update(float dt_, Engine::EntityManager* entityManager_);
		static std::unique_ptr<PlayerController> Create()
		{
			return std::make_unique<PlayerController>();
		}
	};
}
