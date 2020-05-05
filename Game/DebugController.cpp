#include "precomp.h"
#include "DebugController.h"


bool Nitro::DebugController::Init(Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_,
	Engine::WindowData* windowData_, Engine::Renderer* renderer_)
{
	ASSERT(entityManager_ != nullptr, "Must pass a valid entityManager");
	ASSERT(textureManager_ != nullptr, "Must pass a valid texture Manager");
	ASSERT(windowData_ != nullptr, "Must pass a valid windowData");

	
#if 0
	for(int i = 0; i < 100; ++i)
	{
		auto background = Engine::Entity::Create();
		
		
		background->AddComponent<Engine::SpriteComponent>().m_Image = textureManager_->GetTexture("background_debug_texture_grid");
		background->AddComponent<Engine::TransformComponent>(0.f, -600.f * i, 512.f, 512.f);
		entityManager_->AddEntity(std::move(background));
	}
#endif
	
	{
		auto entity = Engine::Entity::Create();
		entity->AddComponent<Engine::SpriteComponent>().m_Image = textureManager_->GetTexture("blank");
		entity->AddComponent<Engine::TransformComponent>(0.f, 0.f, 15.f, 15.f);
		entityManager_->AddEntity(std::move(entity));
	}

	{
		auto entity = Engine::Entity::Create();
		entity->AddComponent<Engine::SpriteComponent>().m_Image = textureManager_->GetTexture("blank");
		entity->AddComponent<Engine::TransformComponent>(20.f, 20.f, 20.f, 20.f);
		entityManager_->AddEntity(std::move(entity));
	}
	
	
	return true;
}

void Nitro::DebugController::Update(float dt_, Engine::EntityManager* entityManager_)
{
}
