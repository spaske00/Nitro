#pragma once


namespace Nitro
{
	class DebugController;
	class CameraController;

	class GameApp : public Engine::Application
	{
		void GameSpecificWindowData() override;
		bool GameSpecificInit() override;
		bool GameSpecificShutdown() override;
		void GameSpecificUpdate(float dt) override;
		std::unique_ptr<CameraController> m_CameraController;

#if _DEBUG
		std::unique_ptr<DebugController> m_DebugController;
#endif
	};
}


inline Engine::Application* Engine::CreateApplication()
{
	return new Nitro::GameApp;
}
