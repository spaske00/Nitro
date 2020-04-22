#pragma once


namespace Nitro
{
	class GameApp : public Engine::Application
	{
		void GameSpecificWindowData() override;
		bool GameSpecificInit() override;
		bool GameSpecificShutdown() override;
		void GameSpecificUpdate(float dt) override;
	};
}


inline Engine::Application* Engine::CreateApplication()
{
	return new Nitro::GameApp;
}
