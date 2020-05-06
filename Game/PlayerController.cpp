#include "precomp.h"
#include "PlayerController.h"
#include "GameComponents.h"
#include "glm/gtx/string_cast.hpp"
#include "glm/gtx/perpendicular.hpp"

namespace Nitro
{
	Engine::Entity* FindRoadTileAtLayer(Engine::Matrix<Engine::Entity*>& tileMatrix, int layer)
	{
		for (int j = 0; j < tileMatrix.Cols(); ++j)
		{
			if (tileMatrix.At(layer, j)->GetComponent<TileInfoComponent>()->m_TileType == TileType::road)
			{
				return tileMatrix.At(layer, j);
			}
		}
		return nullptr;
	}
}


bool Nitro::PlayerController::Init(Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_)
{
	ASSERT(entityManager_ != nullptr, "Must pass a valid entity manager");
	ASSERT(textureManager_ != nullptr, "Must pass a vliad texture manager");

	auto track = entityManager_->GetEntityWithComponent<TrackComponent>();
	ASSERT(track != nullptr, "Track must exists");
	
	auto trackComponent = track->GetComponent<TrackComponent>();
	int lowestLayer = trackComponent->m_LowestLayerIndex;
	auto firstRoadTile = FindRoadTileAtLayer(trackComponent->m_TracksMatrix, lowestLayer);
	auto firstRoadTilePosition = firstRoadTile->GetComponent<Engine::TransformComponent>()->m_Position;
	auto firstRoadTileSize = firstRoadTile->GetComponent<Engine::TransformComponent>()->m_Size;
	
	{
		auto player1 = Engine::Entity::Create();
		player1->AddComponent<Engine::DrawableEntity>();
		
		
		auto& transform = player1->AddComponent<Engine::TransformComponent>(firstRoadTilePosition.x - firstRoadTileSize.x / 4, 
			firstRoadTilePosition.y - firstRoadTileSize.y / 4, 50.f, 50.f);

		player1->AddComponent<Engine::SpriteComponent>().m_Image = textureManager_->GetTexture("player1Texture");
		player1->AddComponent<Engine::CollisionComponent>(50.f);
		player1->AddComponent<Engine::CollidedWithComponent>();
		player1->AddComponent<Engine::MoverComponent>();
		player1->AddComponent<Engine::PlayerComponent>();
		
		auto& input = player1->AddComponent<Engine::InputComponent>();

		input.inputActions.emplace_back("Player1MoveUp");
		input.inputActions.emplace_back("Player1MoveDown");
		input.inputActions.emplace_back("Player1MoveLeft");
		input.inputActions.emplace_back("Player1MoveRight");
		// input.inputActions.emplace_back("Player1Jump");

		player1->AddComponent<PlayerTagComponent>(PlayerTag::One);

		auto& physics = player1->AddComponent<CarPhysicsComponent>();
		
		physics.m_Acceleration = 120.f;
		physics.m_CarSpeed = 0.f;
		physics.m_CarHeading = -M_PI / 2;
		physics.m_SteerAngle = 0.f;
		physics.m_WheelBase = transform.m_Size.y;
		physics.m_BreakSpeed = 25.f;
		physics.m_Drag = 10.f;
		physics.m_MaxSpeed = 800.f;
		physics.m_MinSpeed = 0.f;
		physics.m_SteerSpeed = 800.f;

		entityManager_->AddEntity(std::move(player1));
	}

	{
		auto player2 = Engine::Entity::Create();
		player2->AddComponent<Engine::DrawableEntity>();
		auto& transform = player2->AddComponent<Engine::TransformComponent>(firstRoadTilePosition.x + firstRoadTileSize.x / 4,
			firstRoadTilePosition.y - firstRoadTileSize.y / 4, 50.f, 50.f);
		player2->AddComponent<Engine::SpriteComponent>().m_Image = textureManager_->GetTexture("player2Texture");
		player2->AddComponent<Engine::CollisionComponent>(50.f);
		player2->AddComponent<Engine::CollidedWithComponent>();
		player2->AddComponent<Engine::MoverComponent>();
		player2->AddComponent<Engine::PlayerComponent>();
		
		auto& input = player2->AddComponent<Engine::InputComponent>();

		input.inputActions.emplace_back("Player2MoveUp");
		input.inputActions.emplace_back("Player2MoveDown");
		input.inputActions.emplace_back("Player2MoveLeft");
		input.inputActions.emplace_back("Player2MoveRight");
		// input.inputActions.emplace_back("Player1Jump");

		player2->AddComponent<PlayerTagComponent>(PlayerTag::Two);

		auto& physics = player2->AddComponent<CarPhysicsComponent>();
		physics.m_Acceleration = 120.f;
		physics.m_CarSpeed = 0.f;
		physics.m_CarHeading = -M_PI/2;
		physics.m_SteerAngle = 0.f;
		physics.m_WheelBase = transform.m_Size.y;
		physics.m_BreakSpeed = 25.f;
		physics.m_Drag = 10.f;
		physics.m_MaxSpeed = 800.f;
		physics.m_MinSpeed = 0.f;
		physics.m_SteerSpeed = 800.f;
		entityManager_->AddEntity(std::move(player2));
	}
	

	return true;
}



void Nitro::PlayerController::Update(float dt_, Engine::EntityManager* entityManager_)
{
	auto players = entityManager_->GetAllEntitiesWithComponents<Engine::PlayerComponent>();
	ASSERT(players.size() == 2, "Must be excatly two players");

	if (players[0]->GetComponent<PlayerTagComponent>()->m_PlayerTag == PlayerTag::Two)
	{
		std::swap(players[0], players[1]);
	}
	// NOTE: Don't put this in the loop. This way is easier to handle mutual player
	//       mutual player interaction
	//       
	// Update Player1
	// TODO: Implement collision
	// TODO: Implement jumping

	
	for (int i = 0; i < 2; ++i)
	{
		auto player = players[i];
		auto otherPlayer = players[(i+1) % 2];
		auto physics = player->GetComponent<CarPhysicsComponent>();
		auto mover = player->GetComponent<Engine::MoverComponent>();
		auto input = player->GetComponent<Engine::InputComponent>();
		auto transform = player->GetComponent<Engine::TransformComponent>();
		
		int tag = PlayerTagToInt(player->GetComponent<PlayerTagComponent>()->m_PlayerTag);
		bool moveUp = Engine::InputManager::IsActionActive(input, fmt::format("Player{}MoveUp", tag));
		bool moveDown = Engine::InputManager::IsActionActive(input, fmt::format("Player{}MoveDown", tag));
		bool moveLeft = Engine::InputManager::IsActionActive(input, fmt::format("Player{}MoveLeft", tag));
		bool moveRight = Engine::InputManager::IsActionActive(input, fmt::format("Player{}MoveRight", tag));

		int accelerationDirection = moveUp - moveDown;

		int wheelTurnintDirection = moveRight - moveLeft;
		physics->m_SteerAngle = 0;
		if (wheelTurnintDirection < 0)
		{
			physics->m_SteerAngle = std::max(-45.f, physics->m_SteerAngle - physics->m_SteerSpeed * dt_);
		}
		else if (wheelTurnintDirection > 0)
		{
			physics->m_SteerAngle = std::min(45.f, physics->m_SteerAngle + physics->m_SteerSpeed * dt_);
		}
		
		if (moveUp)
		{
			physics->m_CarSpeed = std::min(physics->m_MaxSpeed, physics->m_CarSpeed + physics->m_Acceleration * dt_);
		}
		else if (moveDown)
		{
			physics->m_CarSpeed = std::max(physics->m_MinSpeed, physics->m_CarSpeed - physics->m_BreakSpeed * dt_);
		}
		else
		{
			physics->m_CarSpeed = std::max(physics->m_MinSpeed, physics->m_CarSpeed - physics->m_Drag * dt_);
		}

		vec2 frontWheel{};
		vec2 backWheel{};

		frontWheel.x = transform->m_Position.x + physics->m_WheelBase / 2 * cos(physics->m_CarHeading);
		frontWheel.y = transform->m_Position.y + physics->m_WheelBase / 2 * sin(physics->m_CarHeading);

		backWheel.x = transform->m_Position.x - physics->m_WheelBase / 2 * cos(physics->m_CarHeading);
		backWheel.y = transform->m_Position.y - physics->m_WheelBase / 2 * sin(physics->m_CarHeading);

		backWheel.x += physics->m_CarSpeed * dt_ * cos(physics->m_CarHeading);
		backWheel.y += physics->m_CarSpeed * dt_ * sin(physics->m_CarHeading);

		frontWheel.x += physics->m_CarSpeed * dt_ * cos(physics->m_CarHeading + DegreesToRadians(physics->m_SteerAngle));
		frontWheel.y += physics->m_CarSpeed * dt_ * sin(physics->m_CarHeading + DegreesToRadians(physics->m_SteerAngle));

		transform->m_Position.x = (frontWheel.x + backWheel.x) / 2;
		transform->m_Position.y = (frontWheel.y + backWheel.y) / 2;

		physics->m_CarHeading = atan2(frontWheel.y - backWheel.y, frontWheel.x - backWheel.x);

		transform->m_Rotation = 90.f + RadiansToDegrees( physics->m_CarHeading);


	}
	
	
}
/*
 * CarPhysics: Jumping, collision, 
 * AI Cars
 * InfoPrint: KM/h, score, koliko zaostaje za ovim drugim igracem...
 * Generisanje Nivoa
 * 
 */