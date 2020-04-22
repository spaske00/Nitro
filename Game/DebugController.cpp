#include "precomp.h"
#include "DebugController.h"


bool Nitro::DebugController::Init(Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_,
	Engine::WindowData* windowData_)
{
	ASSERT(entityManager_ != nullptr, "Must pass a valid entityManager");
	ASSERT(textureManager_ != nullptr, "Must pass a valid texture Manager");
	ASSERT(windowData_ != nullptr, "Must pass a valid windowData");

	{
		
	}
	
	return true;
}

void Nitro::DebugController::Update(float dt_, Engine::EntityManager* entityManager_)
{
}
