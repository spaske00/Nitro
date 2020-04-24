#pragma once


namespace Engine {
	class EntityManager;
}

namespace Nitro
{
	class TrackController
	{
	public:
		bool Init(Engine::EntityManager* entityManager_);
		void Update(float dt_, Engine::EntityManager* entityManager_);
	};

}

