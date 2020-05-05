#pragma once

namespace Engine {
	class EntityManager;
}

namespace Nitro
{
	class CameraController
	{
	public:
		bool Init(Engine::EntityManager* entityManager_, const Engine::WindowData* windowData_);
		void Update(float dt_, Engine::EntityManager* entityManager_);

		static std::unique_ptr<CameraController> Create()
		{
			return std::make_unique<CameraController>();
		}

		const Engine::WindowData* m_WindowData;
	};
}
