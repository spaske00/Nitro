#include "precomp.h"
#include "TrackController.h"





bool Nitro::TrackController::Init(Engine::Renderer* renderer_, Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_)
{
	ASSERT(entityManager_ != nullptr, "Must pass a valid entity manager");

	{
		auto colorMatrix = std::make_shared<Engine::Matrix<Color>>(2, 3);
		auto tileSize = vec2{ 1280 / 3, 720 };
		for (int i = 0; i < colorMatrix->Rows(); ++i)
		{
			colorMatrix->At(i, 0) = Color{ 255, 0, 0, 255 };
			colorMatrix->At(i, 1) = Color{ 0, 255, 0, 255 };
			colorMatrix->At(i, 2) = Color{ 0, 0, 255, 255 };
		}
		
		auto result = Engine::Texture::CreateMatrixOfTexturesFromMatrixOfColors(renderer_, *colorMatrix, tileSize.y, tileSize.x);

		{
			auto backgroundMatrix = Engine::Entity::Create();
			auto& component = backgroundMatrix->AddComponent<Engine::ColorTrackMatrixComponent>();
			component.m_ColorMatrix = colorMatrix;
			component.m_CenterPosition = { tileSize.x * colorMatrix->Cols() / 2, -tileSize.y * colorMatrix->Rows() / 2};
			component.m_TileSize = tileSize;
			component.m_Size = vec2{ tileSize.x * colorMatrix->Rows(), tileSize.y * colorMatrix->Cols() };
			component.m_ScaleFactor = { 1,1 };
			

			for (int i = 0; i < result.Rows(); ++i)
			{
				for (int j = 0; j < result.Cols(); ++j)
				{
					auto background = Engine::Entity::Create();
					auto beginY = component.m_CenterPosition.y - component.m_Size.y / 2;
					background->AddComponent<Engine::TransformComponent>(component.m_CenterPosition.x + j * component.m_CenterPosition.x, -i * beginY + beginY, 720.f, 2560.f);
					background->AddComponent<Engine::SpriteComponent>().m_Image = result.At(i, j).get();
					background->AddComponent<Engine::BackgroundComponent>();
					textureManager_->AddTexture(fmt::format("background{}{}", i, j), std::move(result.At(i, j)));
					entityManager_->AddEntity(std::move(background));
				}
			}
			entityManager_->AddEntity(std::move(backgroundMatrix));
		}
		

		LOG_INFO("Size of matrix {} {}", result.Rows(), result.Cols());
		
	}
	
	
#if 0
	{
		auto background = Engine::Entity::Create();
		background->AddComponent<Engine::TransformComponent>(400.f, -400.f, 800.f, 800.f);
		background->AddComponent < Engine::BackgroundComponent >();
		background->AddComponent<Engine::SpriteComponent>().m_Image = textureManager_->GetTexture("background_debug_texture_grid");

		entityManager_->AddEntity(std::move(background));
	}
#endif
	return true;
}

void Nitro::TrackController::Update(float dt_, Engine::EntityManager* entityManager_)
{
}
