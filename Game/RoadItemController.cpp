#include "precomp.h"
#include "GameComponents.h"
#include "TextureController.h"
#include "RoadItemController.h"

namespace Nitro
{
	/*Engine::Entity* ItemFindRoadTileAtLayer(Engine::Matrix<Engine::Entity*>& tileMatrix, int layer)
	{
		for (int j = 0; j < tileMatrix.Cols(); ++j)
		{
			if (tileMatrix.At(layer, j)->GetComponent<TileInfoComponent>()->m_TileType == TileType::road)
			{
				return tileMatrix.At(layer, j);
			}
		}
		return nullptr;
	}*/

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
			if (tileMatrix.At(x-2, j)->GetComponent<TileInfoComponent>()->m_TileType == TileType::road)
			{
				indexes.push_back(j);
			}
		}
		int l = indexes.size();
		r = rand() % l;

		return tileMatrix.At(x-2, indexes[r]);
	}
}
static int update, minimize;
bool Nitro::RoadItemController::Init(Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_)
{
	ASSERT(entityManager_ != nullptr, "Must pass a valid entity manager");
	ASSERT(textureManager_ != nullptr, "Must pass a valid texture manager");

	ASSERT(entityManager_ != nullptr, "Must pass a valid entity manager");

	auto track = entityManager_->GetEntityWithComponent<TrackComponent>();
	ASSERT(track != nullptr, "Track must exists");

	auto trackComponent = track->GetComponent<TrackComponent>();
	int lowestLayer = trackComponent->m_LowestLayerIndex;

	update = 0;
	minimize = 0;
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
	ASSERT(items.size() == 8, "Eight items!");
	
	int lowestLayer;
	auto cam = cameras[0];

	srand(time(NULL));
	int r = rand() % 10 + 1;

	auto& posCam = cam->GetComponent<Engine::TransformComponent>()->m_Position;
	lowestLayer = trackComponent->m_LowestLayerIndex;

	for(auto item : items)
	{
		auto& posIt = item->GetComponent<Engine::TransformComponent>()->m_Position;
		if ((posIt.y - 600.0f) > posCam.y)
		{
			if (lowestLayer == 35)
			{
				if (update == 0)
				{
					minimize = 9;
				}
				else if (update == 3)
				{
					minimize = 9;
					update = 1;
				}
				else
				{
					minimize += 5;
				}
			}
			else if (lowestLayer == 23)
			{
				if (update == 0) update = 1;
				if (update == 1)
				{
					minimize = 8;
					update = 2;
				}
				else
				{
					minimize += 5;
				}
			}
			else
			{
				if (update == 2)
				{
					minimize = 7;
					update = 3;
				}
				else
				{
					minimize += 3;
				}
			}
			
			lowestLayer = trackComponent->m_LowestLayerIndex;
			if (update != 0)
			{
				int what;
				if (lowestLayer - minimize <= 0) what = 9;
				else what = lowestLayer - minimize;
				auto tile = RandomRoadTileAtLayer(trackComponent->m_TracksMatrix, what);
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
}

bool Nitro::RoadItemController::Compare(Engine::Entity* p1, Engine::Entity* p2)
{
	int y1 = p1->GetComponent<Engine::TransformComponent>()->m_Position.y;
	int y2 = p2->GetComponent<Engine::TransformComponent>()->m_Position.y;
	return (y1 < y2);
}

void Nitro::RoadItemController::ComponentAdd(Engine::Entity* it)
{
	it->AddComponent<Engine::DrawableEntity>();
	it->AddComponent<Engine::CollisionComponent>(50.f, 50.f);
	it->AddComponent<Engine::CollidedWithComponent>();
	it->AddComponent<Engine::RoadItemComponent>();
}
