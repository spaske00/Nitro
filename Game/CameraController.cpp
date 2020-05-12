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

Engine::Entity* GetLeadingPlayer(Engine::Entity* player1, Engine::Entity* player2)
{
	auto t1 = player1->GetComponent<Engine::TransformComponent>();
	auto t2 = player2->GetComponent<Engine::TransformComponent>();
	if (t1->m_Position.y < t2->m_Position.y)
	{
		return player1;
	}
	else
	{
		return player2;
	}
}

bool PlayerOutOfCameraOnTheDownSide(Engine::Entity* player, Engine::Entity* camera)
{
	auto playerPosition = player->GetComponent<Engine::TransformComponent>()->m_Position;
	auto playerSize = player->GetComponent<Engine::TransformComponent>()->m_Size;

	auto cameraPosition = camera->GetComponent<Engine::TransformComponent>()->m_Position;
	auto cameraSize = camera->GetComponent<Engine::TransformComponent>()->m_Size;

	return abs(playerPosition.y - cameraPosition.y) > (abs(cameraSize.y / 2) + (playerSize.x));
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

	auto leadingPlayer = GetLeadingPlayer(player1, player2);
	{
		auto transform = camera->GetComponent<Engine::TransformComponent>();
		/*transform->m_Position.y = (player1Position.y + player2Position.y) / 2;
		transform->m_Position.x = (player1Position.x + player2Position.x) / 2;*/
		
		transform->m_Position.y = leadingPlayer->GetComponent<Engine::TransformComponent>()->m_Position.y;
		transform->m_Position.y -= transform->m_Size.y / 4;

	
		transform->m_Position.x = (player1->GetComponent<Engine::TransformComponent>()->m_Position.x 
			+ player2->GetComponent<Engine::TransformComponent>()->m_Position.x) / 2;	
	}

	if (PlayerOutOfCameraOnTheDownSide(player1, camera))
	{
		player1->GetComponent<PlayerTagComponent>()->m_PlayerState = PlayerState::dead;
	}
	else if (PlayerOutOfCameraOnTheDownSide(player2, camera))
	{
		player2->GetComponent<PlayerTagComponent>()->m_PlayerState = PlayerState::dead;
	}
	

	

}
