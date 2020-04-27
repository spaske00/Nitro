#include "precomp.h"
#include "TextController.h"
#include "GameComponents.h"
#include "TextureController.h"


bool Nitro::TextController::Init(Engine::EntityManager* entityManager_) {

	ASSERT(entityManager_ != nullptr, "Must pass a valid entity manager");

	auto km1 = Engine::Entity::Create();
	km1->AddComponent<Engine::TextComponent>("km/h", 1, -2);
	km1->AddComponent<Engine::TransformComponent>(90.f, 40.f);
	entityManager_->AddEntity(std::move(km1));

	auto km2 = Engine::Entity::Create();
	km2->AddComponent<Engine::TextComponent>("km/h", 2, -2);
	km2->AddComponent<Engine::TransformComponent>(1200.f, 40.f);
	entityManager_->AddEntity(std::move(km2));

	auto score1 = Engine::Entity::Create();
	score1->AddComponent<Engine::TextComponent>("0", 1, -1);
	score1->AddComponent<Engine::TransformComponent>(40.f, 40.f);
	entityManager_->AddEntity(std::move(score1));

	auto score2 = Engine::Entity::Create();
	score2->AddComponent<Engine::TextComponent>("0", 2, -1);
	score2->AddComponent<Engine::TransformComponent>(1150.f, 40.f);
	entityManager_->AddEntity(std::move(score2));

	auto mile1 = Engine::Entity::Create();
	mile1->AddComponent<Engine::TextComponent>("0", 1, 0);
	mile1->AddComponent<Engine::TransformComponent>(40.f, 15.f);
	entityManager_->AddEntity(std::move(mile1));

	auto mile2 = Engine::Entity::Create();
	mile2->AddComponent<Engine::TextComponent>("0", 2, 0);
	mile2->AddComponent<Engine::TransformComponent>(1150.f, 15.f);
	entityManager_->AddEntity(std::move(mile2));

	return true;
}

void Nitro::TextController::Update(float dt_, Engine::EntityManager* entityManager_) {

	ASSERT(entityManager_ != nullptr, "Must pass a valid entity manager");

	auto texts = entityManager_->GetAllEntitiesWithComponent<Engine::TextComponent>();
	auto players = entityManager_->GetAllEntitiesWithComponent<Engine::PlayerComponent>();
	auto player1 = players[0];
	auto player2 = players[1];

	if (player1->GetComponent<PlayerTagComponent>()->m_PlayerTag == PlayerTag::Two)
	{
		std::swap(player1, player2);
	}

	auto speed1 = player1->GetComponent<CarPhysicsComponent>();
	auto speed2 = player2->GetComponent<CarPhysicsComponent>();

	for (auto text : texts) {
		auto tekst = text->GetComponent<Engine::TextComponent>();

		switch (tekst->m_mile) {
		case -1: {
			if (tekst->m_player == 1)
				{
				std::string s = std::to_string((int)speed1->m_Speed);
				tekst->m_text = s;
				break;
				}
			else {
				std::string s = std::to_string((int)speed2->m_Speed);
				tekst->m_text = s;
				break;
				}

			}
		case -2: { break;}
		default: {
			if (tekst->m_player == 1)
				{
				std::string s = std::to_string(56);
				tekst->m_text = s;
				break;
				}
			else {
				std::string s = std::to_string(56);
				tekst->m_text = s;
				break;
				}

			}
		}

	}

}
