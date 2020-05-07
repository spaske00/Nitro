#include "precomp.h"
#include "WorldController.h"

bool Nitro::WorldController::Init(Engine::EntityManager* entityManager_)
{
	ASSERT(entityManager_ != nullptr, "Must pass a valid entity manager");
	
	return true;
}
