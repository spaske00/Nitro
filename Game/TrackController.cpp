#include "precomp.h"
#include "TrackController.h"




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

bool Nitro::TrackController::Init(Engine::Renderer* renderer_, Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_)
{
	ASSERT(entityManager_ != nullptr, "Must pass a valid entity manager");

	PlaceWater(textureManager_, entityManager_,vec2{ -512.f, -512.f * 100 }, vec2{ 512.f * 5, 512.f }, vec2{1024.f, 1024.f});
	
	return true;
}

void Nitro::TrackController::Update(float dt_, Engine::EntityManager* entityManager_)
{
}
