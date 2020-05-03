#pragma once


namespace Engine {
	class EntityManager;
	class Renderer;
	class TextureManager;
}

namespace Nitro
{
	enum class TileType;
	class TrackController
	{
	public:
		bool Init(Engine::Renderer* renderer_, Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_);
		void Update(float dt_, Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_);

	private:
		bool ShouldGenerateNextTileLayer(Engine::Entity* tileMatrix_, Engine::Entity* player1, Engine::Entity* player2);
		
		void MoveTrackLayersFromDownToTheTop(Engine::Entity* trackEntity_, int n);
		
		std::pair<int, int> FindPlayersLayerIndexLocations(Engine::Entity* trackComponent, Engine::Entity* player1, Engine::Entity* player2);
		void PasteTileMatrixChunkOnTheTrack(Engine::Entity* trackEntity_, Engine::TextureManager* textureManager_, const Engine::Matrix<TileType>& tileMatrix);
	};

}

