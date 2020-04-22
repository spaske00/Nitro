#pragma once


namespace Nitro
{
	class CameraController;

	class GameApp : public Engine::Application
	{
		void GameSpecificWindowData() override;
		bool GameSpecificInit() override;
		bool GameSpecificShutdown() override;
		void GameSpecificUpdate(float dt) override;
		std::unique_ptr<CameraController> m_CameraController;
	};
}


inline Engine::Application* Engine::CreateApplication()
{
	return new Nitro::GameApp;
}
