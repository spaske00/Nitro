#pragma once

namespace Engine {
	class EntityManager;
	class TextureManager;
}

namespace Nitro
{

	class TextController
	{
	public:
		bool Init(Engine::EntityManager* entityManager_);
		void Update(float dt_, Engine::EntityManager* entityManager_);
		static std::unique_ptr<TextController> Create() {

			return std::make_unique<TextController>();
		}
		int oldPosition1=-348, oldPosition2= -348;
		int Distance1 = 0, Distance2 = 0;
	};
}