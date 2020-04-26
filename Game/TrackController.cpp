#include "precomp.h"
#include "TrackController.h"

#include "Color.h"



bool Nitro::TrackController::Init(Engine::Renderer* renderer_, Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_)
{
	ASSERT(entityManager_ != nullptr, "Must pass a valid entity manager");

	{
		Engine::Matrix<Engine::ColorA> colorMatrix(64000, 100);
		
		for (int i = 0; i < colorMatrix.Rows(); ++i)
		{
			for(int j = 0; j < colorMatrix.Cols(); ++j)
			{
				colorMatrix.At(i, j) = Engine::ColorA{ rand() % 256u, rand() % 256u, rand() % 256u, 255 };

			}
			/*colorMatrix.At(i, 0) = Engine::ColorA{ 0, 255, 0, 255 };
			colorMatrix.At(i, 1) = Engine::ColorA{ 192, 192, 192, 255 };
			colorMatrix.At(i, 2) = Engine::ColorA{ 192, 192, 192, 255 };
			colorMatrix.At(i, 3) = Engine::ColorA{ 192, 192, 192, 255 };
			colorMatrix.At(i, 4) = Engine::ColorA{ 192, 192, 192, 255 };
			colorMatrix.At(i, 5) = Engine::ColorA{ 0, 0, 255, 255 };*/
		}
		
		auto result = Engine::Texture::CreateMatrixOfTexturesFromMatrixOfColors(renderer_, colorMatrix, 8, 8);

		for(int i = 0; i < result.Rows(); ++i)
		{
			for (int j = 0; j < result.Cols(); ++j)
			{
				auto background = Engine::Entity::Create();
				auto dimensions = result.At(i, j).get()->QueryDimensions();
				background->AddComponent<Engine::TransformComponent>((float)dimensions.width * j, -(float)dimensions.height * i, (float)dimensions.width, (float)dimensions.height);
				background->AddComponent<Engine::SpriteComponent>().m_Image = result.At(i, j).get();
				background->AddComponent<Engine::BackgroundComponent>();
				textureManager_->AddTexture(fmt::format("background{}{}", i, j), std::move(result.At(i, j)));
				entityManager_->AddEntity(std::move(background));
			}
		}
	}
	
	
#if 0
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
#endif
	return true;
}

void Nitro::TrackController::Update(float dt_, Engine::EntityManager* entityManager_)
{
}
