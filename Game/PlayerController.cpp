#include "precomp.h"
#include "PlayerController.h"


bool Nitro::PlayerController::Init(Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_)
{
	ASSERT(entityManager_ != nullptr, "Must pass a valid entity manager");
	ASSERT(textureManager_ != nullptr, "Must pass a vliad texture manager");

	
	{
		auto player1 = Engine::Entity::Create();
		player1->AddComponent<Engine::TransformComponent>(-40.f, 0.f, 50.f, 50.f);
		player1->AddComponent<Engine::SpriteComponent>().m_Image = textureManager_->GetTexture("player1Texture");

		entityManager_->AddEntity(std::move(player1));
	}

	{
		auto player2 = Engine::Entity::Create();
		player2->AddComponent<Engine::TransformComponent>(40.f, 0.f, 50.f, 50.f);
		player2->AddComponent<Engine::SpriteComponent>().m_Image = textureManager_->GetTexture("player2Texture");

		entityManager_->AddEntity(std::move(player2));
	}
	

	return true;
}

void Nitro::PlayerController::Update(float dt_, Engine::EntityManager* entityManager_)
{
}
