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

		int damage = 5;
		int boost = 5;

	private:
		void ComponentAdd(Engine::Entity* it);
		std::pair<int, int> FindItemsLayerIndexLocations(Engine::Entity* trackEntity, Engine::Entity* item);
	};
};

