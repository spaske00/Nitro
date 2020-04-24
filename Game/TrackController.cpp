#include "precomp.h"
#include "TrackController.h"


bool Nitro::TrackController::Init(Engine::EntityManager* entityManager_)
{
	ASSERT(entityManager_ != nullptr, "Must pass a valid entity manager");

	auto background = Engine::Entity::Create();
	auto &tileMap = background->AddComponent<Engine::TileMapComponent>();
	tileMap.m_IndividualTileSize = { 15.f, 15.f };
	entityManager_->AddEntity(std::move(background));

	return true;
}

void Nitro::TrackController::Update(float dt_, Engine::EntityManager* entityManager_)
{
}
