#include "precomp.h"
#include "DebugController.h"


bool Nitro::DebugController::Init(Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_,
	Engine::WindowData* windowData_, Engine::Renderer* renderer_)
{
	ASSERT(entityManager_ != nullptr, "Must pass a valid entityManager");
	ASSERT(textureManager_ != nullptr, "Must pass a valid texture Manager");
	ASSERT(windowData_ != nullptr, "Must pass a valid windowData");

	if (!textureManager_->CreateTexture(renderer_, "background_debug_texture_grid", "Resource/background_debug_texture_grid.png"))
	{
		LOG_ERROR("Failed to load bacgkround_Debug_texture_gird");
		return false;
	}
	for(int i = 0; i < 100; ++i)
	{
		auto background = Engine::Entity::Create();
		
		
		background->AddComponent<Engine::SpriteComponent>().m_Image = textureManager_->GetTexture("background_debug_texture_grid");
		background->AddComponent<Engine::TransformComponent>(0.f, -600.f * i, 512.f, 512.f);
		entityManager_->AddEntity(std::move(background));
	}
	
	return true;
}

void Nitro::DebugController::Update(float dt_, Engine::EntityManager* entityManager_)
{
}
