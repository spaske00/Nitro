#include "precomp.h"
#include "CameraController.h"
#include "GameComponents.h"
bool Nitro::CameraController::Init(Engine::EntityManager* entityManager_, const Engine::WindowData* windowData_)
{
	ASSERT(entityManager_ != nullptr, "Must pass a valid entityManager");

	m_WindowData = windowData_;
	
	{	
		auto camera = Engine::Entity::Create();
		camera->AddComponent<Engine::CameraComponent>();
		camera->AddComponent<Engine::TransformComponent>(windowData_->m_Width / 2.f, windowData_->m_Height / 4.f, static_cast<float>(windowData_->m_Width)*2, static_cast<float>(windowData_->m_Height)*2);
		camera->AddComponent<Engine::MoverComponent>();
		camera->AddComponent<PlayerTagComponent>(PlayerTag::Two);
		entityManager_->AddEntity(std::move(camera));
	}

	return true;
}


void Nitro::CameraController::Update(float dt_, Engine::EntityManager* entityManager_)
{
	ASSERT(entityManager_ != nullptr, "must pass a valid entity manager");
	
	auto players = entityManager_->GetAllEntitiesWithComponents<Engine::PlayerComponent>();
	ASSERT(players.size() == 2, "Must be exactly 2 players");

	auto cameras = entityManager_->GetAllEntitiesWithComponent<Engine::CameraComponent>();
	ASSERT(cameras.size() == 1, "Only one camera");

	auto camera = cameras[0];
	
	auto player1 = players[0];
	auto player2 = players[1];
	if (player1->GetComponent<PlayerTagComponent>()->m_PlayerTag == PlayerTag::Two)
	{
		std::swap(player1, player2);
	}

	auto player1Position = player1->GetComponent<Engine::TransformComponent>()->m_Position;
	auto player2Position = player2->GetComponent<Engine::TransformComponent>()->m_Position;
	
	{
		auto transform = camera->GetComponent<Engine::TransformComponent>();
		/*transform->m_Position.y = (player1Position.y + player2Position.y) / 2;
		transform->m_Position.x = (player1Position.x + player2Position.x) / 2;*/
		
		transform->m_Position = player1Position;

	/*	transform->m_Size.x = m_WindowData->m_Width * abs(2 * sin(totalDt) + 1.f);
		transform->m_Size.y = m_WindowData->m_Height * abs(2 * sin(totalDt) + 1.f);*/
	}
}
