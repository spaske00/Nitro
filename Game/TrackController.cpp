#include "precomp.h"
#include "TrackController.h"

#include "GameComponents.h"


// TODO(Marko): convert all of these to int, no need for floating point operations
void PlaceTrack(const Engine::Matrix<Nitro::TileType>& tileMatrix, Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_,
	vec2 tileSize = {256.f, 512.f})
{
	vec2 upperLeftCorner = { 0.f, -tileSize.y * tileMatrix.Rows() };
	Engine::Matrix<Engine::Entity*> tracksMatrix(tileMatrix.Rows(), tileMatrix.Cols());
	for(int i = 0; i < tileMatrix.Rows(); ++i, upperLeftCorner.y += tileSize.y)
	{
		upperLeftCorner.x = 0;
		for(int j = 0; j < tileMatrix.Cols(); ++j, upperLeftCorner.x += tileSize.x)
		{
			auto tileType = tileMatrix.At(i, j);
			auto entity = Engine::Entity::Create();
			entity->AddComponent<Engine::TransformComponent>(upperLeftCorner.x + tileSize.x / 2, upperLeftCorner.y + tileSize.y / 2, tileSize.x, tileSize.y);
			entity->AddComponent<Engine::BackgroundComponent>();

			entity->AddComponent<Nitro::TileInfoComponent>(tileType);

			auto texture = textureManager_->GetTexture(Nitro::TileTypeToTextureName(tileType));
			ASSERT(texture != nullptr, fmt::format("Couldnt load {}", Nitro::TileTypeToTextureName(tileType)));
			entity->AddComponent <Engine::SpriteComponent>().m_Image = texture;
			tracksMatrix.At(i, j) = entity.get();
			if (tileType == Nitro::TileType::road)
			{
				entity->AddComponent<Engine::CollisionComponent>(tileSize.x, tileSize.y);
			}
			entityManager_->AddEntity(std::move(entity));
		}	
	}
	auto tracksEntity = Engine::Entity::Create();
	auto& trackComponent = tracksEntity->AddComponent<Nitro::TrackComponent>();
	trackComponent.m_TracksMatrix = std::move(tracksMatrix);
	trackComponent.m_TileSize = tileSize;
	trackComponent.m_UpperLeftCornerWorldPosition = { 0.f, -tileSize.y * tileMatrix.Rows() };
	trackComponent.m_LowestLayerIndex = tileMatrix.Rows() - 1;
	entityManager_->AddEntity(std::move(tracksEntity));
}



bool Nitro::TrackController::Init(Engine::Renderer* renderer_, Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_)
{
	ASSERT(entityManager_ != nullptr, "Must pass a valid entity manager");

	Engine::Matrix<TileType> track(8, 12);
	
	for (int i = 0; i < track.Rows(); ++i)
	{
		for (int j = 0; j < track.Cols(); ++j)
		{
			if (j <= 2 || j > 8)
			{
				track.At(i, j) = TileType::water_deep;
			}
			else if(j == 3)
			{
				track.At(i, j) = TileType::water_beach;
			}
			else if(j == 8)
			{
				track.At(i, j) = TileType::beach_water;
			}
			else
			{
				track.At(i, j) = TileType::road;
			}
		}
	}

	vec2 tileSize{ 256.f, 512.f };
	PlaceTrack(track, entityManager_, textureManager_, tileSize);


	return true;
}

void Nitro::TrackController::Update(float dt_, Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_)
{
	ASSERT(entityManager_ != nullptr, "Must pass a valid entityManager");
	
	auto tileMatrixEntities = entityManager_->GetAllEntitiesWithComponent<Nitro::TrackComponent>();
	ASSERT(tileMatrixEntities.size() == 1, "Must be excatly one tileMatrix");
	auto tileMatrix = tileMatrixEntities[0];

	auto playerEntities = entityManager_->GetAllEntitiesWithComponent<Engine::PlayerComponent>();
	ASSERT(playerEntities.size() >= 1, "Must be at least one player");
	auto player1 = playerEntities[0];
	auto player2 = playerEntities[1];
	if (player1->GetComponent<Nitro::PlayerTagComponent>()->m_PlayerTag == PlayerTag::Two)
	{
		std::swap(player1, player2);
	}

	if (ShouldGenerateNextTileLayer(tileMatrix, player1, player2))
	{
		LOG_INFO("NEW LAYER GENERATED");
		GenerateNextTileLayer(tileMatrix, textureManager_);
	}

	
}

int dist(int a, int b, int modulo)
{
	int value = b - a;
	int mod = value % modulo;
	if (value < 0)
	{
		mod += modulo;
	}
	return mod;
}

bool Nitro::TrackController::ShouldGenerateNextTileLayer(Engine::Entity* trackEntity, Engine::Entity* player1, Engine::Entity* player2)
{
	auto trackComponent = trackEntity->GetComponent<TrackComponent>();
	ASSERT(trackComponent, "Track component must exist");
	int lastRow = trackComponent->m_LowestLayerIndex;
	auto [player1IndexLocation, player2IndexLocation] = FindPlayersLayerIndexLocations(trackEntity, player1, player2);
	
	int lowestLayer = trackComponent->m_LowestLayerIndex;
	int totalRows = (int)trackComponent->m_TracksMatrix.Rows();

	int player1LayerDistanceFromLowestLayer = dist(player1IndexLocation, lowestLayer, totalRows);
	int player2LayerDistanceFromLowestLayer = dist(player2IndexLocation, lowestLayer, totalRows);
	//LOG_INFO(fmt::format("LowestLayer {}", lowestLayer));
	//LOG_INFO(fmt::format("Player1 {} {} Player2 {} {}", player1IndexLocation, player1LayerDistanceFromLowestLayer, player2IndexLocation, player2LayerDistanceFromLowestLayer));

	return std::min(player1LayerDistanceFromLowestLayer, player2LayerDistanceFromLowestLayer) > 2;
}

void Nitro::TrackController::GenerateNextTileLayer(Engine::Entity* trackEntity_, Engine::TextureManager* textureManager_)
{
	auto trackComponent = trackEntity_->GetComponent<TrackComponent>();
	int cols = (int)trackComponent->m_TracksMatrix.Cols();
	int rows = (int)trackComponent->m_TracksMatrix.Rows();
	int lowestLayer = trackComponent->m_LowestLayerIndex;
	int highestLayer = (lowestLayer + 1) % rows; // NOTE(Marko) + 1 because layers go from 0 to Rows from upperLeftCorner in the world. Meaning players traverse layers 19, 18, 17... when going forward
	auto& matrix = trackComponent->m_TracksMatrix;
	float highestRowYCenterPosition = matrix.At(highestLayer, 0)->GetComponent<Engine::TransformComponent>()->m_Position.y;
	
	for (int i = 0; i < cols; ++i)
	{
		auto& e = matrix.At(lowestLayer, i);
		auto transform = e->GetComponent<Engine::TransformComponent>();
		transform->m_Position.y = (highestRowYCenterPosition - trackComponent->m_TileSize.y);
	}
	trackComponent->m_LowestLayerIndex = lowestLayer == 0 ? rows - 1 : lowestLayer - 1;
}

std::pair<int, int> Nitro::TrackController::FindPlayersLayerIndexLocations(Engine::Entity* trackEntity, Engine::Entity* player1, Engine::Entity* player2)
{
	std::pair<int, int> result{ -1,-1 };
	auto trackComponent = trackEntity->GetComponent<TrackComponent>();
	auto tracksMatrix = trackComponent->m_TracksMatrix;
	auto trackRows = trackComponent->m_TracksMatrix.Rows();
	vec2 tileSize = trackComponent->m_TileSize;
	auto player1Transform = player1->GetComponent<Engine::TransformComponent>();
	auto player2Transform = player2->GetComponent<Engine::TransformComponent>();
	for (int i = 0; i < trackRows; ++i)
	{
		auto ithTileLayerRowCenterYCoordiante = tracksMatrix.At(i, 0)->GetComponent<Engine::TransformComponent>()->m_Position.y;
		if (abs(player1Transform->m_Position.y - ithTileLayerRowCenterYCoordiante) < tileSize.y / 2)
		{
			result.first = i;
		}
		if (abs(player2Transform->m_Position.y - ithTileLayerRowCenterYCoordiante) < tileSize.y / 2)
		{
			result.second = i;
		}
	}

	return result;
}
