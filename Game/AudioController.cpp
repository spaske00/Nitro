#include "precomp.h"
#include "AudioController.h"

bool Nitro::AudioController::Init(Engine::AudioManager* audioManager_)
{
	ASSERT(audioManager_ != nullptr, "Must pass a valid audioManager");
	if (!audioManager_->LoadMusic("E:\\Nitro\\Game\\Resource\\Sound\\music.ogg", "background_music"))
	{
		LOG_WARNING("Failed to load music.ogg");
		return false;
	}
	audioManager_->PlayMusic("background_music");
	return true;
}

void Nitro::AudioController::Update(float dt, Engine::AudioManager* audioManager_)
{
}
