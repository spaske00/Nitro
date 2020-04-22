#include "precomp.h"
#include "CameraController.h"

bool Nitro::CameraController::Init(Engine::EntityManager* entityManager_, const Engine::WindowData* windowData_)
{
	ASSERT(entityManager_ != nullptr, "Must pass a valid entityManager");

	{
		auto camera = Engine::Entity::Create();
		camera->AddComponent<Engine::CameraComponent>().m_OnScreenPositionOffset = {0.f, 0.f};
		camera->AddComponent<Engine::TransformComponent>(0.f, 0.f, static_cast<float>(windowData_->m_Width / 2.f), static_cast<float>(windowData_->m_Height));
		camera->AddComponent<Engine::MoverComponent>();
		entityManager_->AddEntity(std::move(camera));
	}
	{
		auto camera = Engine::Entity::Create();
		camera->AddComponent<Engine::CameraComponent>().m_OnScreenPositionOffset = {windowData_->m_Width / 2.f, 0.f};
		camera->AddComponent<Engine::TransformComponent>(0.f, 0.f, static_cast<float>(windowData_->m_Width / 2.f), static_cast<float>(windowData_->m_Height));
		camera->AddComponent<Engine::MoverComponent>();
		entityManager_->AddEntity(std::move(camera));
	}

	return true;
}

void Nitro::CameraController::Update(float dt_, Engine::EntityManager* entityManager_)
{
}
