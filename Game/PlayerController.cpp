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
	auto firstRoadTile = FindRoadTileAtLayer(trackComponent->m_TracksMatrix, lowestLayer - 6);
	auto firstRoadTilePosition = firstRoadTile->GetComponent<Engine::TransformComponent>()->m_Position;
	auto firstRoadTileSize = firstRoadTile->GetComponent<Engine::TransformComponent>()->m_Size;
	
	for (int i = 1; i <= 2; ++i)
	{
		auto player = Engine::Entity::Create();
		player->AddComponent<Engine::DrawableEntity>();

		
		auto& transform = player->AddComponent<Engine::TransformComponent>(firstRoadTilePosition.x - firstRoadTileSize.x / 4,
			firstRoadTilePosition.y - firstRoadTileSize.y / 4, 50.f, 50.f);

		if (i == 2)
		{
			transform.m_Position = vec2{ firstRoadTilePosition.x + firstRoadTileSize.x / 4,
			firstRoadTilePosition.y - firstRoadTileSize.y / 4 };
		}
		

		player->AddComponent<Engine::SpriteComponent>().m_Image = textureManager_->GetTexture(fmt::format("player{}Texture", i));
		player->AddComponent<Engine::CollisionComponent>(40.f);
		player->AddComponent<Engine::CollidedWithComponent>();
		player->AddComponent<Engine::MoverComponent>();
		player->AddComponent<Engine::PlayerComponent>();

		auto& jumping = player->AddComponent<JumpingComponent>();
		jumping.m_InTheAir = false;
		jumping.m_AirbornTimeLeft = -1.f;
		jumping.m_JumpTimeCooldownLeft = -1.f;
		jumping.m_JumpTimeCooldownLength = 4.f;
		jumping.m_JumpTimeLength = 1.8f;
		jumping.m_OriginalModelSize = transform.m_Size;

		auto& input = player->AddComponent<Engine::InputComponent>();

		input.inputActions.emplace_back(fmt::format("Player{}MoveUp", i));
		input.inputActions.emplace_back(fmt::format("Player{}MoveDown", i));
		input.inputActions.emplace_back(fmt::format("Player{}MoveLeft", i));
		input.inputActions.emplace_back(fmt::format("Player{}MoveRight", i));
		input.inputActions.emplace_back(fmt::format("Player{}Jump", i));

		// input.inputActions.emplace_back("Player1Jump");

		player->AddComponent<PlayerTagComponent>(PlayerTagFromInt(i));

		auto& physics = player->AddComponent<CarPhysicsComponent>();

		physics.m_Acceleration = 120.f;
		physics.m_CarSpeed = 0.f;
		physics.m_CarHeading = -(float)M_PI / 2;
		physics.m_SteerAngle = 0.f;
		physics.m_WheelBase = transform.m_Size.y;
		physics.m_BreakSpeed = 300.f;
		physics.m_Drag = 40.f;
		physics.m_MaxSpeed = 800.f;
		physics.m_MinSpeed = 0.f;
		physics.m_SteerSpeed = 15.f;

		entityManager_->AddEntity(std::move(player));
	}

	return true;
}




void Nitro::PlayerController::Update(float dt_, Engine::EntityManager* entityManager_, Engine::AudioManager* audioManager_)
{
	auto players = entityManager_->GetAllEntitiesWithComponents<Engine::PlayerComponent>();
	ASSERT(players.size() == 2, "Must be excatly two players");

	if (players[0]->GetComponent<PlayerTagComponent>()->m_PlayerTag == PlayerTag::Two)
	{
		std::swap(players[0], players[1]);
	}
	
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
		HandleJump(dt_, jump, player, audioManager_);
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
void Nitro::PlayerController::HandleJump(float dt_, bool jump, Engine::Entity* player, Engine::AudioManager* audioManager_)
{
	if (auto jumpingComponent = player->GetComponent<JumpingComponent>())
	{
		if (jumpingComponent->m_InTheAir == false && jump && jumpingComponent->m_JumpTimeCooldownLeft < 0.f)
		{
			jumpingComponent->m_AirbornTimeLeft = jumpingComponent->m_JumpTimeLength;
			jumpingComponent->m_InTheAir = true;
			jumpingComponent->m_JumpTimeCooldownLeft = jumpingComponent->m_JumpTimeCooldownLength;
			player->GetComponent<Engine::SpriteComponent>()->m_RenderPriority = Engine::RenderPriorty::Top;
			audioManager_->PlaySoundEffect("jump_sound");
		}

		if (jumpingComponent->m_InTheAir)
		{
			if (jumpingComponent->m_AirbornTimeLeft > 0.f)
			{
				auto transform = player->GetComponent<Engine::TransformComponent>();
				transform->m_Size = jumpingComponent->m_OriginalModelSize + jumpingComponent->m_OriginalModelSize * sin((float)M_PI * jumpingComponent->m_AirbornTimeLeft / jumpingComponent->m_JumpTimeLength);
				jumpingComponent->m_AirbornTimeLeft -= dt_;
			}
			else
			{
				jumpingComponent->m_InTheAir = false;
				player->GetComponent<Engine::SpriteComponent>()->m_RenderPriority = Engine::RenderPriorty::Normal;
			}
		}
		else
		{
			jumpingComponent->m_JumpTimeCooldownLeft -= dt_;
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