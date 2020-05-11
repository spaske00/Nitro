#pragma once
namespace Engine
{
	class TextureManager;
	class EntityManager;
}

namespace Nitro
{
	class RoadItemController
	{
	public:
		bool Init(Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_);
		void Update(float dt_, Engine::EntityManager* entityManager_);
		
		static std::unique_ptr<RoadItemController> Create()
		{
			return std::make_unique<RoadItemController>();
		}

	private:
		void ComponentAdd(Engine::Entity* it);
		bool Compare(Engine::Entity* p1, Engine::Entity* p2);
	};
};

