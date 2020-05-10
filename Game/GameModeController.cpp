#include "precomp.h"
#include "GameModeController.h"
#include "TextureController.h"
#include "TextController.h"
#include "GameComponents.h"

namespace Nitro {
	bool Nitro::GameModeController::Init(Engine::EntityManager* entityManager_)
	{
		ASSERT(entityManager_ != nullptr, "Must pass a valid entity manager");

		m_GameMode = GameMode::MenuMode;
		auto players = entityManager_->GetAllEntitiesWithComponent<Engine::PlayerComponent>();
		auto player1 = players[0];
		auto player2 = players[1];
		if (player1->GetComponent<PlayerTagComponent>()->m_PlayerTag == PlayerTag::Two)
		{
			std::swap(player1, player2);
		}


		auto start = Engine::Entity::Create();
		start->AddComponent<Engine::TextComponent>("PRESS ENTER OR Q TO START");
		start->AddComponent<Engine::TransformComponent>(400, 300.f);
		start->AddComponent<TextInfoComponent>(PlayerTag::One, TextInfoType::Start, player1);
		entityManager_->AddEntity(std::move(start));

		auto pause = Engine::Entity::Create();
		pause->AddComponent<Engine::TextComponent>(" ");
		pause->AddComponent<Engine::TransformComponent>(300, 50.f);
		pause->AddComponent<TextInfoComponent>(PlayerTag::Two, TextInfoType::Pause, player2);
		entityManager_->AddEntity(std::move(pause));

		auto score = Engine::Entity::Create();
		score->AddComponent<Engine::TextComponent>(" ");
		score->AddComponent<Engine::TransformComponent>(400, 200.f);
		score->AddComponent<TextInfoComponent>(PlayerTag::Two, TextInfoType::Score, player2);
		entityManager_->AddEntity(std::move(score));

		//auto players = entityManager_->GetAllEntitiesWithComponents<Engine::PlayerComponent>();

		return true;
	}

	void Nitro::GameModeController::Update(float dt, Engine::EntityManager* entityManager_, Engine::AudioManager* audioManager_, Engine::InputManager* inputManager_)
	{
		ASSERT(entityManager_ != nullptr, "Must pass a valid entity manager");

		auto texts = entityManager_->GetAllEntitiesWithComponent<Engine::TextComponent>();
		auto players = entityManager_->GetAllEntitiesWithComponents<Engine::PlayerComponent>();
		
		if (players[0]->GetComponent<PlayerTagComponent>()->m_PlayerTag == PlayerTag::Two)
		{
			std::swap(players[0], players[1]);
		}

		for (auto text : texts) {
			auto tekst = text->GetComponent<Engine::TextComponent>();
			auto info = text->GetComponent<TextInfoComponent>();

			switch (m_GameMode) {
			case GameMode::MenuMode: {


				if (info->m_Type == TextInfoType::Start && (inputManager_->IsActionInState("Start1Game", Engine::EInputActionState::Released) || inputManager_->IsActionInState("Start2Game", Engine::EInputActionState::Released))) {
					m_GameMode = GameMode::PlayingMode;
					audioManager_->PlayMusic("background_music");
					
					tekst->m_text = " ";
				}

			}break;
			case GameMode::PlayingMode: {

				if (info->m_Type == TextInfoType::Pause && (inputManager_->IsActionInState("Pause1Game", Engine::EInputActionState::Released) || inputManager_->IsActionInState("Pause2Game", Engine::EInputActionState::Released))) {
					m_GameMode = GameMode::PauseMode;
					audioManager_->PauseMusic();
					tekst->m_text = "PAUSE II";
				}

				
				
				if (players[0]->GetComponent<PlayerTagComponent>()->m_PlayerState == PlayerState::dead || players[1]->GetComponent<PlayerTagComponent>()->m_PlayerState == PlayerState::dead) {
					audioManager_->PlaySoundEffect("finish_sound");
					audioManager_->StopMusic();
					audioManager_->PlayMusic("score_music");
					m_GameMode = GameMode::ScoreMode;
				}
				
			}break;
			case GameMode::PauseMode: {

				if (info->m_Type == TextInfoType::Pause && (inputManager_->IsActionInState("Pause1Game", Engine::EInputActionState::Released) || inputManager_->IsActionInState("Pause2Game", Engine::EInputActionState::Released))) {
					m_GameMode = GameMode::PlayingMode;
					audioManager_->ResumeMusic();
					tekst->m_text = " ";
				}
			}break;
			case GameMode::ScoreMode: {
				auto winner = players[0]->GetComponent<PlayerTagComponent>()->m_PlayerState == PlayerState::alive ? players[0] : players[1];
				std::string player = winner->GetComponent<PlayerTagComponent>()->m_PlayerTag == PlayerTag::One ? "BLUE PLAYER WON" : "RED PLAYER WON";
				auto winner_jump = winner->GetComponent<JumpingComponent>();

				if (info->m_Type == TextInfoType::Score) {
					if (winner_jump->m_InTheAir == false && winner_jump->m_JumpTimeCooldownLeft < 0.f)
					{
						winner_jump->m_AirbornTimeLeft = winner_jump->m_JumpTimeLength;
						winner_jump->m_InTheAir = true;
						winner_jump->m_JumpTimeCooldownLeft = winner_jump->m_JumpTimeCooldownLength;
						winner->GetComponent<Engine::SpriteComponent>()->m_RenderPriority = Engine::RenderPriorty::Top;
					}
					if (winner_jump->m_InTheAir)
					{
						if (winner_jump->m_AirbornTimeLeft > 0.f)
						{
							auto transform = winner->GetComponent<Engine::TransformComponent>();
							transform->m_Size = winner_jump->m_OriginalModelSize + winner_jump->m_OriginalModelSize * sin((float)M_PI * winner_jump->m_AirbornTimeLeft / winner_jump->m_JumpTimeLength);
							winner_jump->m_AirbornTimeLeft -= dt;
						}
						else
						{
							winner_jump->m_InTheAir = false;
							winner->GetComponent<Engine::SpriteComponent>()->m_RenderPriority = Engine::RenderPriorty::Normal;
						}
					}
					else
					{
						winner_jump->m_JumpTimeCooldownLeft -= dt;
					}
					tekst->m_text = " THE GAME IS FINISHED "+player;
				}
			}break;
			};

		}
	}

};