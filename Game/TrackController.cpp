#include "precomp.h"
#include "TrackController.h"

#include "Color.h"



bool Nitro::TrackController::Init(Engine::Renderer* renderer_, Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_)
{
	ASSERT(entityManager_ != nullptr, "Must pass a valid entity manager");

	{
		Engine::Matrix<Engine::ColorA> colorMatrix(100, 3);
		
		for (int i = 0; i < colorMatrix.Rows(); ++i)
		{
			colorMatrix.At(i, 0) = Engine::ColorA{ 255, 0, 0, 255 };
			colorMatrix.At(i, 1) = Engine::ColorA{ 0, 255, 0, 255 };
			colorMatrix.At(i, 2) = Engine::ColorA{ 0, 0, 255, 255 };
		}
		auto result = Engine::Texture::CreateMatrixOfTexturesFromMatrixOfColors(renderer_, colorMatrix, 1280, 200);

		for(int i = 0; i < result.Rows(); ++i)
		{
			for (int j = 0; j < result.Cols(); ++j)
			{
				auto background = Engine::Entity::Create();
				background->AddComponent<Engine::TransformComponent>(100 + j * 200.f, -i * 1280.f, 600.f, 1280.f);
				background->AddComponent<Engine::SpriteComponent>().m_Image = result.At(i, j).get();
				background->AddComponent<Engine::BackgroundComponent>();
				textureManager_->AddTexture(fmt::format("background{}{}", i, j), std::move(result.At(i, j)));
				entityManager_->AddEntity(std::move(background));
			}
		}


		LOG_INFO("Size of matrix {} {}", result.Rows(), result.Cols());
		
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
