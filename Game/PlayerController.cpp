#include "precomp.h"
#include "PlayerController.h"
#include "GameComponents.h"

bool Nitro::PlayerController::Init(Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_)
{
	ASSERT(entityManager_ != nullptr, "Must pass a valid entity manager");
	ASSERT(textureManager_ != nullptr, "Must pass a vliad texture manager");

	
	{
		auto player1 = Engine::Entity::Create();
		player1->AddComponent<Engine::TransformComponent>(-40.f, 0.f, 50.f, 50.f);
		player1->AddComponent<Engine::SpriteComponent>().m_Image = textureManager_->GetTexture("player1Texture");
		player1->AddComponent<Engine::CollisionComponent>(50.f, 50.f);
		player1->AddComponent<Engine::MoverComponent>();
		player1->AddComponent<Engine::PlayerComponent>();
		auto& input = player1->AddComponent<Engine::InputComponent>();

		input.inputActions.emplace_back("Player1MoveUp");
		input.inputActions.emplace_back("Player1MoveDown");
		input.inputActions.emplace_back("Player1MoveLeft");
		input.inputActions.emplace_back("Player1MoveRight");
		// input.inputActions.emplace_back("Player1Jump");

		player1->AddComponent<PlayerTagComponent>(PlayerTag::One);
		
		entityManager_->AddEntity(std::move(player1));
	}

	{
		auto player2 = Engine::Entity::Create();
		
		player2->AddComponent<Engine::TransformComponent>(40.f, 0.f, 50.f, 50.f);
		player2->AddComponent<Engine::SpriteComponent>().m_Image = textureManager_->GetTexture("player2Texture");
		player2->AddComponent<Engine::CollisionComponent>(50.f, 50.f);
		player2->AddComponent<Engine::MoverComponent>();
		player2->AddComponent<Engine::PlayerComponent>();
		auto& input = player2->AddComponent<Engine::InputComponent>();

		input.inputActions.emplace_back("Player2MoveUp");
		input.inputActions.emplace_back("Player2MoveDown");
		input.inputActions.emplace_back("Player2MoveLeft");
		input.inputActions.emplace_back("Player2MoveRight");
		// input.inputActions.emplace_back("Player1Jump");

		player2->AddComponent<PlayerTagComponent>(PlayerTag::Two);
		entityManager_->AddEntity(std::move(player2));
	}
	

	return true;
}



void Nitro::PlayerController::Update(float dt_, Engine::EntityManager* entityManager_)
{
	auto players = entityManager_->GetAllEntitiesWithComponents<Engine::PlayerComponent>();
	ASSERT(players.size() == 2, "Must be excatly two players");
	for (auto player : players)
	{
		
		auto mover = player->GetComponent<Engine::MoverComponent>();
		auto input = player->GetComponent<Engine::InputComponent>();
		auto speed = 500.f;

		int tag = PlayerTagToInt(player->GetComponent<PlayerTagComponent>()->m_PlayerTag);
		bool moveUp = Engine::InputManager::IsActionActive(input, fmt::format("Player{}MoveUp", tag));
		bool moveDown = Engine::InputManager::IsActionActive(input, fmt::format("Player{}MoveDown", tag));
		bool moveLeft = Engine::InputManager::IsActionActive(input, fmt::format("Player{}MoveLeft", tag));
		bool moveRight = Engine::InputManager::IsActionActive(input, fmt::format("Player{}MoveRight", tag));

		mover->m_TranslationSpeed = speed * vec2{ (moveRight ? 1.f : 0.f) + (moveLeft ? -1.f : 0.f), (moveUp ? -1.f : 0.f) + (moveDown ? 1.f : 0.f) };
	}
}
