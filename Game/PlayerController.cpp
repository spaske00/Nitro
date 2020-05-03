#include "precomp.h"
#include "PlayerController.h"
#include "GameComponents.h"

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
		
		
		player1->AddComponent<Engine::TransformComponent>(firstRoadTilePosition.x - firstRoadTileSize.x / 4, 
			firstRoadTilePosition.y - firstRoadTileSize.y / 4, 50.f, 50.f);

		player1->AddComponent<Engine::SpriteComponent>().m_Image = textureManager_->GetTexture("player1Texture");
		player1->AddComponent<Engine::CollisionComponent>(50.f, 50.f);
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
		physics.m_Mass = 300.f;
		physics.m_Acceleration = 1.f;
		physics.m_Speed = 0.f;
		
		entityManager_->AddEntity(std::move(player1));
	}

	{
		auto player2 = Engine::Entity::Create();
		player2->AddComponent<Engine::DrawableEntity>();
		player2->AddComponent<Engine::TransformComponent>(firstRoadTilePosition.x + firstRoadTileSize.x / 4,
			firstRoadTilePosition.y - firstRoadTileSize.y / 4, 50.f, 50.f);
		player2->AddComponent<Engine::SpriteComponent>().m_Image = textureManager_->GetTexture("player2Texture");
		player2->AddComponent<Engine::CollisionComponent>(50.f, 50.f);
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
		physics.m_Mass = 300.f;
		physics.m_Acceleration = 1.f;
		physics.m_Speed = 0.f;

		entityManager_->AddEntity(std::move(player2));
	}
	

	return true;
}



void Nitro::PlayerController::Update(float dt_, Engine::EntityManager* entityManager_)
{
	auto players = entityManager_->GetAllEntitiesWithComponents<Engine::PlayerComponent>();
	ASSERT(players.size() == 2, "Must be excatly two players");
	auto player1 = players[0];
	auto player2 = players[1];
	if (player1->GetComponent<PlayerTagComponent>()->m_PlayerTag == PlayerTag::Two)
	{
		std::swap(player1, player2);
	}
	// NOTE: Don't put this in the loop. This way is easier to handle mutual player
	//       mutual player interaction
	//       
	// Update Player1
	// TODO: Implement collision
	// TODO: Implement jumping
	{
		auto player = player1;
		auto otherPlayer = player2;
		auto physics = player->GetComponent<CarPhysicsComponent>();
		auto mover = player->GetComponent<Engine::MoverComponent>();
		auto input = player->GetComponent<Engine::InputComponent>();
		
		
		int tag = PlayerTagToInt(player->GetComponent<PlayerTagComponent>()->m_PlayerTag);
		bool moveUp = Engine::InputManager::IsActionActive(input, fmt::format("Player{}MoveUp", tag));
		bool moveDown = Engine::InputManager::IsActionActive(input, fmt::format("Player{}MoveDown", tag));
		bool moveLeft = Engine::InputManager::IsActionActive(input, fmt::format("Player{}MoveLeft", tag));
		bool moveRight = Engine::InputManager::IsActionActive(input, fmt::format("Player{}MoveRight", tag));

		int accelerationDirection = moveUp - moveDown;

		physics->m_Speed += accelerationDirection * physics->m_Acceleration;
		physics->m_Speed = std::max(0.f, physics->m_Speed);
		physics->m_Speed = std::min(480.f, physics->m_Speed);
		mover->m_TranslationSpeed = physics->m_Speed * vec2{ (moveRight ? 1.f : 0.f) + (moveLeft ? -1.f : 0.f), physics->m_Speed > 0 ? -1.f : 0.f};
	}

	// Update player2
	{
		auto player = player2;
		auto otherPlayer = player1;
		auto physics = player->GetComponent<CarPhysicsComponent>();
		auto mover = player->GetComponent<Engine::MoverComponent>();
		auto input = player->GetComponent<Engine::InputComponent>();


		int tag = PlayerTagToInt(player->GetComponent<PlayerTagComponent>()->m_PlayerTag);
		bool moveUp = Engine::InputManager::IsActionActive(input, fmt::format("Player{}MoveUp", tag));
		bool moveDown = Engine::InputManager::IsActionActive(input, fmt::format("Player{}MoveDown", tag));
		bool moveLeft = Engine::InputManager::IsActionActive(input, fmt::format("Player{}MoveLeft", tag));
		bool moveRight = Engine::InputManager::IsActionActive(input, fmt::format("Player{}MoveRight", tag));

		int accelerationDirection = moveUp - moveDown;

		physics->m_Speed += accelerationDirection * physics->m_Acceleration;
		physics->m_Speed = std::max(0.f, physics->m_Speed);
		physics->m_Speed = std::min(480.f, physics->m_Speed);
		mover->m_TranslationSpeed = physics->m_Speed * vec2{ (moveRight ? 1.f : 0.f) + (moveLeft ? -1.f : 0.f), physics->m_Speed > 0 ? -1.f : 0.f };
	}

	auto collided = player1->GetComponent<Engine::CollidedWithComponent>()->m_CollidedWith;
	
	
}
/*
 * CarPhysics: Jumping, collision, 
 * AI Cars
 * InfoPrint: KM/h, score, koliko zaostaje za ovim drugim igracem...
 * Generisanje Nivoa
 * 
 */