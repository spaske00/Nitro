#include "precomp.h"
#include "TrackController.h"

#include "GameComponents.h"


void PlaceWater(Engine::TextureManager* textureManager_, Engine::EntityManager* entityManager_, vec2 upperLeftCorner, vec2 lowerRightCorner, vec2 textureSize_ = {0.f,0.f})
{
	ASSERT(upperLeftCorner.x < lowerRightCorner.x && upperLeftCorner.y < lowerRightCorner.y, "Must lowerLeft and upperRight corner");
	auto texture = textureManager_->GetTexture("water");
	Engine::Texture::Dimensions textureDimensions;
	if (textureSize_ == vec2{0,0})
	{
		textureDimensions = texture->QueryDimensions();
	}
	else
	{
		textureDimensions.width = textureSize_.x;
		textureDimensions.height = textureSize_.y;
	}
	
	for (float x = upperLeftCorner.x; x <= lowerRightCorner.x; x += textureDimensions.width)
	{
		// because negative y axis goes up on the screen
		for (float y = upperLeftCorner.y; y <= lowerRightCorner.y; y += textureDimensions.height)
		{
			vec2 texturePosition = { x + textureDimensions.width / 2, y + textureDimensions.height / 2 };
			vec2 textureSize = { (float)textureDimensions.width, (float)textureDimensions.height };
			
			auto entity = Engine::Entity::Create();
			entity->AddComponent<Engine::SpriteComponent>().m_Image = texture;
			entity->AddComponent<Engine::BackgroundComponent>();
			entity->AddComponent<Engine::TransformComponent>(texturePosition, textureSize);
			
			entityManager_->AddEntity(std::move(entity));
		}
	}
}


void PlaceRoad(Engine::TextureManager* textureManager_, Engine::EntityManager* entityManager_, vec2 upperLeftCorner, int countX, int countY, vec2 textureSize_ = { 0.f,0.f })
{
	ASSERT(countX >= 0 && countY >= 0, "Must have positive number of textures");
	auto texture = textureManager_->GetTexture("road");
	Engine::Texture::Dimensions textureDimensions;
	if (textureSize_ == vec2{ 0,0 })
	{
		textureDimensions = texture->QueryDimensions();
	}
	else
	{
		textureDimensions.width = textureSize_.x;
		textureDimensions.height = textureSize_.y;
	}


	int i = 0;
	
	for (float x = upperLeftCorner.x; i < countX; x += textureDimensions.width, ++i)
	{
		// because negative y axis goes up on the screen
		int j = 0;
		for (float y = upperLeftCorner.y; j < countY; y += textureDimensions.height, ++j)
		{
			vec2 texturePosition = { x + textureDimensions.width / 2, y + textureDimensions.height / 2 };
			vec2 textureSize = { (float)textureDimensions.width, (float)textureDimensions.height };

			auto entity = Engine::Entity::Create();
			entity->AddComponent<Engine::SpriteComponent>().m_Image = texture;
			entity->AddComponent<Engine::BackgroundComponent>();
			entity->AddComponent<Engine::TransformComponent>(texturePosition, textureSize);
			entity->AddComponent<Engine::CollisionComponent>(textureSize.x, textureSize.y);
			
			entityManager_->AddEntity(std::move(entity));
		}
	}
}





void PlaceRoad(Engine::TextureManager* textureManager_, Engine::EntityManager* entityManager_, vec2 upperLeftCorner, vec2 lowerRightCorner, vec2 textureSize_ = { 0.f,0.f })
{
	ASSERT(upperLeftCorner.x < lowerRightCorner.x && upperLeftCorner.y < lowerRightCorner.y, "Must lowerLeft and upperRight corner");
	auto texture = textureManager_->GetTexture("road");
	Engine::Texture::Dimensions textureDimensions;
	if (textureSize_ == vec2{ 0,0 })
	{
		textureDimensions = texture->QueryDimensions();
	}
	else
	{
		textureDimensions.width = textureSize_.x;
		textureDimensions.height = textureSize_.y;
	}


	for (float x = upperLeftCorner.x; x <= lowerRightCorner.x; x += textureDimensions.width)
	{
		// because negative y axis goes up on the screen
		for (float y = upperLeftCorner.y; y <= lowerRightCorner.y; y += textureDimensions.height)
		{
			vec2 texturePosition = { x + textureDimensions.width / 2, y + textureDimensions.height / 2 };
			vec2 textureSize = { (float)textureDimensions.width, (float)textureDimensions.height };

			auto entity = Engine::Entity::Create();
			entity->AddComponent<Engine::SpriteComponent>().m_Image = texture;
			entity->AddComponent<Engine::BackgroundComponent>();
			entity->AddComponent<Engine::TransformComponent>(texturePosition, textureSize);
			entity->AddComponent<Engine::CollisionComponent>(textureSize.x, textureSize.y);

			
			entityManager_->AddEntity(std::move(entity));
		}
	}
}
// TODO(Marko): convert all of these to int, no need for floating point operations
void PlaceTrack(const Engine::Matrix<Nitro::TileType>& tileMatrix, Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_,
	vec2 tileSize = {256.f, 512.f})
{
	vec2 upperLeftCorner = { 0.f, -tileSize.y * tileMatrix.Rows() };
	
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
			
			if (tileType == Nitro::TileType::road)
			{
				entity->AddComponent<Engine::CollisionComponent>(tileSize.x, tileSize.y);
			}
			entityManager_->AddEntity(std::move(entity));
		}	
	}
}



bool Nitro::TrackController::Init(Engine::Renderer* renderer_, Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_)
{
	ASSERT(entityManager_ != nullptr, "Must pass a valid entity manager");

	Engine::Matrix<TileType> track(20, 12);
	
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

	vec2 tileSize{ 256.f, 2048.f };
	PlaceTrack(track, entityManager_, textureManager_, tileSize);


	return true;
}

void Nitro::TrackController::Update(float dt_, Engine::EntityManager* entityManager_)
{
}
