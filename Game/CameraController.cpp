#include "precomp.h"
#include "CameraController.h"
#include "GameComponents.h"
bool Nitro::CameraController::Init(Engine::EntityManager* entityManager_, const Engine::WindowData* windowData_)
{
	ASSERT(entityManager_ != nullptr, "Must pass a valid entityManager");

	{
		auto camera = Engine::Entity::Create();
		camera->AddComponent<Engine::CameraComponent>().m_OnScreenPositionOffset = {0.f, 0.f};
		camera->AddComponent<Engine::TransformComponent>(0.f, 0.f, static_cast<float>(windowData_->m_Width / 2.f), static_cast<float>(windowData_->m_Height));
		camera->AddComponent<Engine::MoverComponent>();
		camera->AddComponent<PlayerTagComponent>(PlayerTag::One);
		
		entityManager_->AddEntity(std::move(camera));
	}
	{	
		auto camera = Engine::Entity::Create();
		camera->AddComponent<Engine::CameraComponent>().m_OnScreenPositionOffset = {windowData_->m_Width / 2.f, 0.f};
		camera->AddComponent<Engine::TransformComponent>(0.f, 0.f, static_cast<float>(windowData_->m_Width / 2.f), static_cast<float>(windowData_->m_Height));
		camera->AddComponent<Engine::MoverComponent>();
		camera->AddComponent<PlayerTagComponent>(PlayerTag::Two);
		entityManager_->AddEntity(std::move(camera));
	}

	return true;
}


void Nitro::CameraController::Update(float dt_, Engine::EntityManager* entityManager_)
{
	ASSERT(entityManager_ != nullptr, "must pass a valid entity manager");
	auto cameras = entityManager_->GetAllEntitiesWithComponents<Engine::CameraComponent>();
	ASSERT(cameras.size() == 2, "Must be exactly 2 cameras");

	auto players = entityManager_->GetAllEntitiesWithComponents<Engine::PlayerComponent>();
	ASSERT(players.size() == 2, "Must be exactly 2 players");

	auto player1Camera = cameras[0];
	auto player2Camera = cameras[1];
	if (player1Camera->GetComponent<PlayerTagComponent>()->m_PlayerTag == PlayerTag::Two)
	{
		std::swap(player1Camera, player2Camera);
	}

	auto player1 = players[0];
	auto player2 = players[1];
	if (player1->GetComponent<PlayerTagComponent>()->m_PlayerTag == PlayerTag::Two)
	{
		std::swap(player1, player2);
	}
	// Snap camera1 to player1
	{
		auto transform = player1Camera->GetComponent<Engine::TransformComponent>();
		transform->m_Position.y = player1->GetComponent<Engine::TransformComponent>()->m_Position.y
		- player1Camera->GetComponent<Engine::TransformComponent>()->m_Size.y / 3;
	}

	// snap camera2 to player2
	{
		auto transform = player2Camera->GetComponent<Engine::TransformComponent>();
		transform->m_Position.y = player2->GetComponent<Engine::TransformComponent>()->m_Position.y - player2Camera->GetComponent<Engine::TransformComponent>()->m_Size.y / 3;
	}
	
}
