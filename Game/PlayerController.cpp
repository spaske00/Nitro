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
		player1->AddComponent<Engine::CollisionComponent>(40.f);
		player1->AddComponent<Engine::CollidedWithComponent>();
		player1->AddComponent<Engine::MoverComponent>();
		player1->AddComponent<Engine::PlayerComponent>();
		player1->AddComponent<JumpingComponent>().m_OriginalModelSize = transform.m_Size;
		auto& input = player1->AddComponent<Engine::InputComponent>();

		input.inputActions.emplace_back("Player1MoveUp");
		input.inputActions.emplace_back("Player1MoveDown");
		input.inputActions.emplace_back("Player1MoveLeft");
		input.inputActions.emplace_back("Player1MoveRight");
		input.inputActions.emplace_back("Player1Jump");

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
		physics.m_SteerSpeed = 15.f;

		entityManager_->AddEntity(std::move(player1));
	}

	{
		auto player2 = Engine::Entity::Create();
		player2->AddComponent<Engine::DrawableEntity>();
		auto& transform = player2->AddComponent<Engine::TransformComponent>(firstRoadTilePosition.x + firstRoadTileSize.x / 4,
			firstRoadTilePosition.y - firstRoadTileSize.y / 4, 50.f, 50.f);
		player2->AddComponent<Engine::SpriteComponent>().m_Image = textureManager_->GetTexture("player2Texture");
		player2->AddComponent<Engine::CollisionComponent>(40.f);
		player2->AddComponent<Engine::CollidedWithComponent>();
		player2->AddComponent<Engine::MoverComponent>();
		player2->AddComponent<Engine::PlayerComponent>();
		player2->AddComponent<JumpingComponent>().m_OriginalModelSize = transform.m_Size;

		auto& input = player2->AddComponent<Engine::InputComponent>();

		input.inputActions.emplace_back("Player2MoveUp");
		input.inputActions.emplace_back("Player2MoveDown");
		input.inputActions.emplace_back("Player2MoveLeft");
		input.inputActions.emplace_back("Player2MoveRight");
		input.inputActions.emplace_back("Player2Jump");
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
		physics.m_SteerSpeed = 15.f;
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
		bool jump = Engine::InputManager::IsActionActive(input, fmt::format("Player{}Jump", tag));


		MoveWheel(dt_, moveLeft, moveRight, physics);
		HandleGasAndBreaking(dt_, moveUp, moveDown, physics);
		SteerTheCar(dt_, player);
		HandleJump(dt_, jump, player);
		CollideWithOtherEntities(dt_, player);

		

	}

}




void Nitro::PlayerController::MoveWheel(float dt_, bool moveLeft, bool moveRight, CarPhysicsComponent* physics)
{
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
}

void Nitro::PlayerController::HandleGasAndBreaking(float dt_, bool moveUp, bool moveDown, CarPhysicsComponent* physics)
{
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

}

void Nitro::PlayerController::SteerTheCar(float dt_, Engine::Entity* player)
{
	auto physics = player->GetComponent<CarPhysicsComponent>();
	auto mover = player->GetComponent<Engine::MoverComponent>();
	auto transform = player->GetComponent<Engine::TransformComponent>();

	vec2 frontWheel{};
	vec2 backWheel{};

	frontWheel.x = transform->m_Position.x + physics->m_WheelBase / 2 * cos(physics->m_CarHeading);
	frontWheel.y = transform->m_Position.y + physics->m_WheelBase / 2 * sin(physics->m_CarHeading);

	backWheel.x = transform->m_Position.x - physics->m_WheelBase / 2 * cos(physics->m_CarHeading);
	backWheel.y = transform->m_Position.y - physics->m_WheelBase / 2 * sin(physics->m_CarHeading);

	backWheel.x += physics->m_CarSpeed * cos(physics->m_CarHeading);
	backWheel.y += physics->m_CarSpeed * sin(physics->m_CarHeading);

	frontWheel.x += physics->m_CarSpeed * cos(physics->m_CarHeading + DegreesToRadians(physics->m_SteerAngle));
	frontWheel.y += physics->m_CarSpeed * sin(physics->m_CarHeading + DegreesToRadians(physics->m_SteerAngle));



	vec2 newTransform = { (frontWheel.x + backWheel.x) / 2 , (frontWheel.y + backWheel.y) / 2 };
	mover->m_TranslationSpeed = newTransform - transform->m_Position;

	physics->m_CarHeading = atan2(frontWheel.y - backWheel.y, frontWheel.x - backWheel.x);

	transform->m_Rotation = 90.f + RadiansToDegrees(physics->m_CarHeading);
}

void Nitro::PlayerController::CollideWithOtherEntities(float dt_, Engine::Entity* player)
{
	auto collidedWithComponent = player->GetComponent<Engine::CollidedWithComponent>();
	auto playerCarPhysics = player->GetComponent<CarPhysicsComponent>();
	for (auto entity : collidedWithComponent->m_CollidedWith)
	{
		if (entity->HasComponent<CarPhysicsComponent>())
		{
			auto entityCarPhysics = entity->GetComponent<CarPhysicsComponent>();

			entity->GetComponent<Engine::CollidedWithComponent>()->m_CollidedWith.erase(player);
		}
	}
}
void Nitro::PlayerController::HandleJump(float dt_, bool jump, Engine::Entity* player)
{
	if (auto jumpingComponent = player->GetComponent<JumpingComponent>())
	{
		if (jumpingComponent->m_InTheAir == false && jump)
		{
			jumpingComponent->m_AirbornEndTime = jumpingComponent->m_JumpTimeLength;
			jumpingComponent->m_InTheAir = true;
		}

		if (jumpingComponent->m_InTheAir)
		{
			if (jumpingComponent->m_AirbornEndTime > 0.f)
			{
				auto transform = player->GetComponent<Engine::TransformComponent>();
				transform->m_Size = jumpingComponent->m_OriginalModelSize + jumpingComponent->m_OriginalModelSize * sin((float)M_PI * jumpingComponent->m_AirbornEndTime / jumpingComponent->m_JumpTimeLength);
				jumpingComponent->m_AirbornEndTime -= dt_;
			}
			else
			{
				jumpingComponent->m_InTheAir = false;
			}
		}
		
	}
}
/*
 * CarPhysics: Jumping, collision, 
 * AI Cars
 * InfoPrint: KM/h, score, koliko zaostaje za ovim drugim igracem...
 * Generisanje Nivoa
 * 
 */