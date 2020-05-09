#pragma once

namespace Engine {
	class EntityManager;
	class TextureManager;
	class InputManager;

}

namespace Nitro {
	enum class GameMode;
	class GameModeController {
	public:
		bool Init(Engine::EntityManager* entityManager_);
		void Update(float dt, Engine::EntityManager* entityManager_, Engine::AudioManager* audioManager_, Engine::InputManager* inputManager_);
		static std::unique_ptr<GameModeController> Create() {
			return std::make_unique<GameModeController>();
		}
		GameMode getGameMode() { return m_GameMode; }
		GameMode m_GameMode;
	};

}