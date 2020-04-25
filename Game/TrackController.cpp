#include "precomp.h"
#include "TrackController.h"

#include "Color.h"



bool Nitro::TrackController::Init(Engine::Renderer* renderer_, Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_)
{
	ASSERT(entityManager_ != nullptr, "Must pass a valid entity manager");
#if 0
	{
		Engine::Matrix<Engine::TileType> colorMatrix(10000, 200);
		
		for (unsigned x = 0; x < colorMatrix.Rows(); ++x)
		{
			for (unsigned y = 0; y < colorMatrix.Cols(); ++y)
			{
				colorMatrix.At(x, y) = { x % 256u, x % 256u, x % 256u, 255 };
			}
		}

		if (!textureManager_->CreateTextureFromColorTileMatrix(renderer_, "background", colorMatrix, 8, 8))
		{
			LOG_ERROR("Failed to create backgruond");
			return false;
		}

		auto background = Engine::Entity::Create();
		background->AddComponent<Engine::TransformComponent>(0, 0, (int)colorMatrix.Rows() * 8, (int)colorMatrix.Cols() * 8);
		background->AddComponent<Engine::BackgroundComponent>();
		background->AddComponent<Engine::SpriteComponent>().m_Image = textureManager_->GetTexture("background");

		entityManager_->AddEntity(std::move(background));
	}

#endif
	{
		if (!textureManager_->CreateTexture(renderer_, "backgroundCoordinate", "Resource/backgroundCoordinate.jpg"))
		{
			LOG_ERROR("Failed to create backgruondCoordinate");
			return false;
		}
		auto background = Engine::Entity::Create();
		background->AddComponent<Engine::TransformComponent>(0, 0, 2000, 2000);
		background->AddComponent<Engine::BackgroundComponent>();
		background->AddComponent<Engine::SpriteComponent>().m_Image = textureManager_->GetTexture("backgroundCoordinate");

		entityManager_->AddEntity(std::move(background));
	}
	return true;
}

void Nitro::TrackController::Update(float dt_, Engine::EntityManager* entityManager_)
{
}
