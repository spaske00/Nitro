#pragma once
namespace Nitro
{


	class DebugController
	{
	public:
		bool Init(Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_,
		          Engine::WindowData* windowData_, Engine::Renderer* renderer_);

		void Update(float dt_, Engine::EntityManager* entityManager_);
		static std::unique_ptr<DebugController> Create()
		{
			return std::make_unique<DebugController>();
		}
	};

}
