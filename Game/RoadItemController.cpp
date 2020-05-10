#include "precomp.h"
#include "GameComponents.h"
#include "TextureController.h"
#include "RoadItemController.h"

namespace Nitro
{


	Engine::Entity* RandomRoadTileAtLayer(Engine::Matrix<Engine::Entity*>& tileMatrix, int layer)
	{
		std::vector<int> indexes{};
		int r;
		for (int j = 0; j < tileMatrix.Cols(); ++j)
		{
			if (tileMatrix.At(layer, j)->GetComponent<TileInfoComponent>()->m_TileType == TileType::road)
			{
				indexes.push_back(j);
			}
		}
		srand(time(NULL));
		int l = indexes.size();
		r = rand() % l;

		return tileMatrix.At(layer, indexes[r]);
	}

	Engine::Entity* RandomRoadTileAtLayerY(Engine::Matrix<Engine::Entity*>& tileMatrix, std::pair<int, int> tileItem)
	{
		std::vector<int> indexes{};
		srand(time(NULL));
		int y = tileItem.second;
		int x = tileItem.first;
		if (x < 2) x += 10;

		int r;
		for (int j = 0; j < tileMatrix.Cols(); ++j)
		{
			if (tileMatrix.At(x - 2, j)->GetComponent<TileInfoComponent>()->m_TileType == TileType::road)
			{
				indexes.push_back(j);
			}
		}
		int l = indexes.size();
		r = rand() % l;

		return tileMatrix.At(x - 2, indexes[r]);
	}
}

bool Nitro::RoadItemController::Init(Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_)
{

	ASSERT(entityManager_ != nullptr, "Must pass a valid entity manager");
	ASSERT(textureManager_ != nullptr, "Must pass a valid texture manager");

	ASSERT(entityManager_ != nullptr, "Must pass a valid entity manager");

	auto track = entityManager_->GetEntityWithComponent<TrackComponent>();
	ASSERT(track != nullptr, "Track must exists");

	auto trackComponent = track->GetComponent<TrackComponent>();
	int lowestLayer = trackComponent->m_LowestLayerIndex;

	int layerStart = 10;

	for (int i = 0; i < 8; i++)
	{
		auto item = Engine::Entity::Create();

		auto tile = RandomRoadTileAtLayer(trackComponent->m_TracksMatrix, lowestLayer - layerStart);
		auto tilePos = tile->GetComponent<Engine::TransformComponent>()->m_Position;
		auto tileSize = tile->GetComponent<Engine::TransformComponent>()->m_Size;

		ComponentAdd(item.get());

		if (i % 2 == 0)
		{
			item->AddComponent<Engine::SpriteComponent>().m_Image = textureManager_->GetTexture("barrier");
			item->AddComponent<RoadItemTagComponent>(RoadItemTag::Bump);
			auto& transform = item->AddComponent<Engine::TransformComponent>(tilePos.x - tileSize.x / 4,
				tilePos.y - tileSize.y / 4, 50.f, 50.f);
			layerStart += 3;
		}
		else
		{
			item->AddComponent<Engine::SpriteComponent>().m_Image = textureManager_->GetTexture("booster");
			item->AddComponent<RoadItemTagComponent>(RoadItemTag::Boost);
			auto& transform = item->AddComponent<Engine::TransformComponent>(tilePos.x + tileSize.x / 4,
				tilePos.y - tileSize.y / 4, 50.f, 50.f);
			layerStart += 4;
		}

		entityManager_->AddEntity(std::move(item));
	}

	return true;
}

void::Nitro::RoadItemController::Update(float dt_, Engine::EntityManager* entityManager_)
{
	ASSERT(entityManager_ != nullptr, "must pass a valid entity manager");

	auto players = entityManager_->GetAllEntitiesWithComponents<Engine::PlayerComponent>();
	ASSERT(players.size() == 2, "Must be exactly 2 players");
	auto cameras = entityManager_->GetAllEntitiesWithComponent<Engine::CameraComponent>();
	ASSERT(cameras.size() == 1, "Only one camera");

	auto track = entityManager_->GetEntityWithComponent<TrackComponent>();
	auto trackComponent = track->GetComponent<TrackComponent>();

	auto items = entityManager_->GetAllEntitiesWithComponents<Engine::RoadItemComponent>();
	ASSERT(items.size() == 8, "Eight bumpers!");


	int lowestLayer, maxY;
	std::pair<int, int> itemTile;
	auto player1 = players[0];
	auto player2 = players[1];
	auto cam = cameras[0];

	srand(time(NULL));
	int r = rand() % 10 + 1;

	auto& posCam = cam->GetComponent<Engine::TransformComponent>()->m_Position;

	for (auto item : items)
	{
		auto& posIt = item->GetComponent<Engine::TransformComponent>()->m_Position;
		if ((posIt.y - 1000.0f) > posCam.y)
		{
			lowestLayer = trackComponent->m_LowestLayerIndex;
			itemTile = FindItemsLayerIndexLocations(track, items.front());

			auto tile = RandomRoadTileAtLayerY(trackComponent->m_TracksMatrix, itemTile);
			auto tilePos = tile->GetComponent<Engine::TransformComponent>()->m_Position;
			auto tileSize = tile->GetComponent<Engine::TransformComponent>()->m_Size;


			if (r % 2 == 0)
			{
				posIt.x = tilePos.x - tileSize.x / 4;
			}
			else
			{
				posIt.x = tilePos.x + tileSize.x / 4;
			}

			if (r < 5)
			{
				posIt.y = tilePos.y - tileSize.y / 4;
			}
			else
			{
				posIt.y = tilePos.y + tileSize.y / 4;
			}

			r = rand() % 10 + 1;
		}
	}
}

void Nitro::RoadItemController::ComponentAdd(Engine::Entity* it)
{
	it->AddComponent<Engine::DrawableEntity>();
	it->AddComponent<Engine::CollisionComponent>(50.f, 50.f);
	it->AddComponent<Engine::CollidedWithComponent>();
	it->AddComponent<Engine::RoadItemComponent>();
}

std::pair<int, int> Nitro::RoadItemController::FindItemsLayerIndexLocations(Engine::Entity* trackEntity, Engine::Entity* item)
{
	std::pair<int, int> result{ -1,-1 };
	auto trackComponent = trackEntity->GetComponent<TrackComponent>();
	auto tracksMatrix = trackComponent->m_TracksMatrix;
	auto trackRows = trackComponent->m_TracksMatrix.Rows();
	vec2 tileSize = trackComponent->m_TileSize;
	auto it = item->GetComponent<Engine::TransformComponent>();
	for (int i = 0; i < trackRows; ++i)
	{
		auto ithTileLayerRowCenterYCoordiante = tracksMatrix.At(i, 0)->GetComponent<Engine::TransformComponent>()->m_Position.y;
		if (abs(it->m_Position.y - ithTileLayerRowCenterYCoordiante) < tileSize.y / 2)
		{
			result.first = i;
		}
	}
	return result;
}
