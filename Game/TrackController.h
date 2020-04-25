#pragma once


namespace Engine {
	class EntityManager;
	class Renderer;
	class TextureManager;
}

namespace Nitro
{
	class TrackController
	{
	public:
		bool Init(Engine::Renderer* renderer_, Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_);
		void Update(float dt_, Engine::EntityManager* entityManager_);
	};

}

