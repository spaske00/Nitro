#include "precomp.h"
#include "GameComponents.h"
#include "TrackPatternGenerator.h"
#include "TrackController.h"



// TODO(Marko): convert all of these to int, no need for floating point operations
void PlaceTrack(const Engine::Matrix<Nitro::TileType>& tileMatrix, Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_,
	vec2 tileSize, int mainTrackColumnBegin, int mainTrackColumnEnd)
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
			// entity->AddComponent<Engine::CollisionComponent>(tileSize.x, tileSize.y);
			entity->AddComponent<Nitro::TileInfoComponent>(tileType);

			auto texture = textureManager_->GetTexture(Nitro::TileTypeToTextureName(tileType));
			ASSERT(texture != nullptr, fmt::format("Couldnt load {}", Nitro::TileTypeToTextureName(tileType)));
			entity->AddComponent <Engine::SpriteComponent>().m_Image = texture;



			tracksMatrix.At(i, j) = entity.get();
			entityManager_->AddEntity(std::move(entity));
		}	
	}
	auto tracksEntity = Engine::Entity::Create();
	auto& trackComponent = tracksEntity->AddComponent<Nitro::TrackComponent>();
	trackComponent.m_TracksMatrix = std::move(tracksMatrix);
	trackComponent.m_TileSize = tileSize;
	trackComponent.m_UpperLeftCornerWorldPosition = { 0.f, -tileSize.y * tileMatrix.Rows() };
	trackComponent.m_LowestLayerIndex = tileMatrix.Rows() - 1;

	// TODO(Marko): HARDCODED!
	trackComponent.m_TrackLeftColumnBoundaryBegin = mainTrackColumnBegin;
	trackComponent.m_TrackRightColumnBoundaryEnd = mainTrackColumnEnd;
	entityManager_->AddEntity(std::move(tracksEntity));
}



bool Nitro::TrackController::Init(Engine::Renderer* renderer_, Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_)
{
	ASSERT(entityManager_ != nullptr, "Must pass a valid entity manager");

	if (!m_TrackPatternGenerator.Init(12, 7))
	{
		LOG_INFO("Track pattern generator failed to init");
		return false;
	}

	Engine::Matrix<TileType> track(36, 21);
	std::fill(std::begin(track), std::end(track), TileType::water);

	

	vec2 tileSize{ 256.f, 256.f };
	int mainTrackColumnBegin = 7;
	int mainTrackColumnEnd = mainTrackColumnBegin + m_TrackPatternGenerator.Cols();
	for (int i = 0; i < track.Rows(); ++i)
	{
		track.At(i, mainTrackColumnBegin - 2) = TileType::water_water_beach;
		track.At(i, mainTrackColumnBegin - 1) = TileType::beach_beach_grass;
		track.At(i, mainTrackColumnEnd) = TileType::grass_beach_beach;
		track.At(i, mainTrackColumnEnd+1) = TileType::beach_water_water;
	}
	PlaceTrack(track, entityManager_, textureManager_, tileSize, mainTrackColumnBegin, mainTrackColumnEnd);

	for (int i = 0; i < 3; ++i)
	{
		PasteTileMatrixChunkOnTheTrackToLayer(entityManager_->GetEntityWithComponent<Nitro::TrackComponent>(),
			textureManager_, m_TrackPatternGenerator.GetNext(), i * m_TrackPatternGenerator.Rows());
	}

	
	return true;
}

void Nitro::TrackController::Update(float dt_, Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_)
{
	ASSERT(entityManager_ != nullptr, "Must pass a valid entityManager");
	
	auto tileMatrixEntities = entityManager_->GetAllEntitiesWithComponent<Nitro::TrackComponent>();
	ASSERT(tileMatrixEntities.size() == 1, "Must be excatly one trackEntity");
	auto trackEntity = tileMatrixEntities[0];

	auto playerEntities = entityManager_->GetAllEntitiesWithComponent<Engine::PlayerComponent>();
	ASSERT(playerEntities.size() >= 1, "Must be at least one player");
	auto player1 = playerEntities[0];
	auto player2 = playerEntities[1];
	if (player1->GetComponent<Nitro::PlayerTagComponent>()->m_PlayerTag == PlayerTag::Two)
	{
		std::swap(player1, player2);
	}

	
	if (ShouldGenerateNextTileLayer(trackEntity, player1, player2))
	{
		LOG_INFO("NEW LAYER GENERATED");
		//GenerateNextTileLayer(tileMatrix, textureManager_);
		MoveTrackLayersFromDownToTheTop(trackEntity, m_TrackPatternGenerator.Rows());
		PasteTileMatrixChunkOnTheTrack(trackEntity, textureManager_, m_TrackPatternGenerator.GetNext());
		
	}
}



bool Nitro::TrackController::ShouldGenerateNextTileLayer(Engine::Entity* trackEntity, Engine::Entity* player1, Engine::Entity* player2)
{
	auto trackComponent = trackEntity->GetComponent<TrackComponent>();
	ASSERT(trackComponent, "Track component must exist");
	int lastRow = trackComponent->m_LowestLayerIndex;
	auto [player1IndexLocation, player2IndexLocation] = FindPlayersLayerIndexLocations(trackEntity, player1, player2);
	
	int lowestLayer = trackComponent->m_LowestLayerIndex;
	int totalRows = (int)trackComponent->m_TracksMatrix.Rows();

	int player1LayerDistanceFromLowestLayer = ModuloDist(player1IndexLocation, lowestLayer, totalRows);
	int player2LayerDistanceFromLowestLayer = ModuloDist(player2IndexLocation, lowestLayer, totalRows);
	
	//LOG_INFO(fmt::format("LowestLayer {}", lowestLayer));
	//LOG_INFO(fmt::format("Player1 {} {} Player2 {} {}", player1IndexLocation, player1LayerDistanceFromLowestLayer, player2IndexLocation, player2LayerDistanceFromLowestLayer));

	// TODO(Marko) :Hardcoded > 14!
	return std::min(player1LayerDistanceFromLowestLayer, player2LayerDistanceFromLowestLayer) > 14;
}


void Nitro::TrackController::MoveTrackLayersFromDownToTheTop(Engine::Entity* trackEntity_, int n)
{
	auto trackComponent = trackEntity_->GetComponent<TrackComponent>();
	auto& trackMatrix = trackComponent->m_TracksMatrix;
	int lowestLayer = trackComponent->m_LowestLayerIndex;
	
	
	for (int i = 0; i < n; ++i)
	{
		int highestLayer = ModuloIncrement(lowestLayer, trackMatrix.Rows());
		float y = trackMatrix.At(highestLayer, 0)->GetComponent<Engine::TransformComponent>()->m_Position.y - trackComponent->m_TileSize.y;

		for (int j = 0; j < trackMatrix.Cols(); ++j)
		{
			trackMatrix.At(lowestLayer, j)->GetComponent<Engine::TransformComponent>()->m_Position.y = y;
			trackMatrix.At(lowestLayer, j)->RemoveComponent<Engine::CollisionComponent>();
		}
		lowestLayer = ModuloDecrement(lowestLayer, trackMatrix.Rows());
	}
	trackComponent->m_LowestLayerIndex = lowestLayer;

}



void Nitro::TrackController::PasteTileMatrixChunkOnTheTrackToLayer(Engine::Entity* trackEntity_, Engine::TextureManager* textureManager_,
	const Engine::Matrix<TileType>& tileMatrix, int toLayer)
{
	auto trackComponent = trackEntity_->GetComponent<TrackComponent>();
	int cols = (int)trackComponent->m_TracksMatrix.Cols();
	int rows = (int)trackComponent->m_TracksMatrix.Rows();
	int lowestLayer = trackComponent->m_LowestLayerIndex;


	auto& tracksMatrix = trackComponent->m_TracksMatrix;
	int highestLayer = toLayer;
	int columnBegin = trackComponent->m_TrackLeftColumnBoundaryBegin;
	int columnEnd = trackComponent->m_TrackRightColumnBoundaryEnd;
	for (int i = 0;
		i < (int)tileMatrix.Rows();
		++i)
	{
		for (int j = 0, trackJ = columnBegin;
			j < (int)tileMatrix.Cols();
			++j, ++trackJ)
		{
			tracksMatrix.At(highestLayer, trackJ)->GetComponent<TileInfoComponent>()->m_TileType = tileMatrix.At(i, j);
			tracksMatrix.At(highestLayer, trackJ)->GetComponent<Engine::SpriteComponent>()->m_Image = textureManager_->GetTexture(TileTypeToTextureName(tileMatrix.At(i, j)));
			// TODO(Marko): SOlve collision component
			if (tileMatrix.At(i, j) == TileType::road)
			{
				auto size = tracksMatrix.At(highestLayer, trackJ)->GetComponent<Engine::TransformComponent>()->m_Size;
				tracksMatrix.At(highestLayer, trackJ)->AddComponent<Engine::CollisionComponent>(size.x, size.y);
			}

			ASSERT(tracksMatrix.At(highestLayer, trackJ)->GetComponent<Engine::SpriteComponent>()->m_Image != nullptr, "Must find a texture");
		}
		highestLayer = ModuloIncrement(highestLayer, tracksMatrix.Rows());
	}

}



void Nitro::TrackController::PasteTileMatrixChunkOnTheTrack(Engine::Entity* trackEntity_, Engine::TextureManager* textureManager_,
	const Engine::Matrix<TileType>& tileMatrix)
{
	auto trackComponent = trackEntity_->GetComponent<TrackComponent>();
	int cols = (int)trackComponent->m_TracksMatrix.Cols();
	int rows = (int)trackComponent->m_TracksMatrix.Rows();
	int lowestLayer = trackComponent->m_LowestLayerIndex;

	
	auto& tracksMatrix = trackComponent->m_TracksMatrix;
	int highestLayer = ModuloIncrement(lowestLayer, tracksMatrix.Rows());
	int columnBegin = trackComponent->m_TrackLeftColumnBoundaryBegin;
	int columnEnd = trackComponent->m_TrackRightColumnBoundaryEnd;
	for (int i = 0;
		i < (int)tileMatrix.Rows();
		++i)
	{
		for (int j = 0, trackJ = columnBegin;
			j < (int)tileMatrix.Cols();
			++j, ++trackJ)
		{
			tracksMatrix.At(highestLayer, trackJ)->GetComponent<TileInfoComponent>()->m_TileType = tileMatrix.At(i, j);
			tracksMatrix.At(highestLayer, trackJ)->GetComponent<Engine::SpriteComponent>()->m_Image = textureManager_->GetTexture(TileTypeToTextureName(tileMatrix.At(i, j)));
			// TODO(Marko): SOlve collision component
			if (tileMatrix.At(i, j) == TileType::road)
			{
				auto size = tracksMatrix.At(highestLayer, trackJ)->GetComponent<Engine::TransformComponent>()->m_Size;
				tracksMatrix.At(highestLayer, trackJ)->AddComponent<Engine::CollisionComponent>(size.x, size.y);
			}
			ASSERT(tracksMatrix.At(highestLayer, trackJ)->GetComponent<Engine::SpriteComponent>()->m_Image != nullptr, "Must find a texture");
		}
		highestLayer = ModuloIncrement(highestLayer, tracksMatrix.Rows());
	}

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