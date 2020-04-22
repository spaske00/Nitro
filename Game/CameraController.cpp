#include "precomp.h"
#include "CameraController.h"

bool Nitro::CameraController::Init(Engine::EntityManager* entityManager_, const Engine::WindowData* windowData_)
{
	ASSERT(entityManager_ != nullptr, "Must pass a valid entityManager");

	{
		auto camera = Engine::Entity::Create();
		camera->AddComponent<Engine::CameraComponent>();
		camera->AddComponent<Engine::TransformComponent>(0.f, 0.f, static_cast<float>(windowData_->m_Height), static_cast<float>(windowData_->m_Width));
		camera->AddComponent<Engine::MoverComponent>();
		entityManager_->AddEntity(std::move(camera));
	}

	return true;
}

void Nitro::CameraController::Update(float dt_, Engine::EntityManager* entityManager_)
{
}
