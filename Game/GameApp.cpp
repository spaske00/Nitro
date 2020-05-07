#include "precomp.h"
#include "GameApp.h"

#include <Core/EntryPoint.h>

#include "GameComponents.h"
#include "TrackPatternGenerator.h"
#include "CameraController.h"
#include "DebugController.h"
#include "PlayerController.h"
#include "TextureController.h"
#include "TrackController.h"
#include "TextController.h"
#include "AudioController.h"

void Nitro::GameApp::GameSpecificWindowData()
{
	Engine::WindowData windowData;
	windowData.m_Height = 720;
	windowData.m_Width = 1280;
	windowData.m_Title = "Nitro";
	SetWindowData(windowData);
}

bool Nitro::GameApp::GameSpecificInit()
{
	m_TextureController = TextureController::Create();
	if (!m_TextureController->Init(m_RenderSystem->GetRenderer(), m_TextureManager.get(), "Resource"))
	{
		LOG_ERROR("Failed to initialize TextureController");
		return false;
	}
	
#if _DEBUG
	m_DebugController = DebugController::Create();
	if (!m_DebugController->Init(m_EntityManager.get(), m_TextureManager.get(), 
	                             &m_WindowData, m_RenderSystem->GetRenderer()))
	{
		LOG_ERROR("Failed to initialize DebugController");
		return false;
	}
#endif

	m_TrackController = TrackController::Create();
	if (!m_TrackController->Init(m_RenderSystem->GetRenderer(), m_EntityManager.get(), m_TextureManager.get()))
	{
		LOG_ERROR("Failed to initialize track controller");
		return false;
	}
	
	m_CameraController = CameraController::Create();
	if (!m_CameraController->Init(m_EntityManager.get(), &m_WindowData))
	{
		LOG_ERROR("Failed to initialize CameraController");
		return false;
	}

	m_PlayerController = PlayerController::Create();
	if (!m_PlayerController->Init(m_EntityManager.get(), m_TextureManager.get()))
	{
		LOG_ERROR("Failed to initialize PlayerController");
		return false;
	}

	m_TextController = TextController::Create();
	if (!m_TextController->Init(m_EntityManager.get()))
	{
		LOG_ERROR("Failed to initialize TextController");
		return false;
	}

	m_AudioController = AudioController::Create();
	if (!m_AudioController->Init(m_AudioManager.get()))
	{
		LOG_ERROR("Failed to initilize AUdioController");
		return false;
	}


	return true;
}

bool Nitro::GameApp::GameSpecificShutdown()
{
	return true;
}

void Nitro::GameApp::GameSpecificUpdate(float dt)
{
#if _DEBUG
	m_DebugController->Update(dt, m_EntityManager.get());
#endif
	m_PlayerController->Update(dt, m_EntityManager.get(), m_AudioManager.get());
	m_TextController->Update(dt, m_EntityManager.get());
	m_CameraController->Update(dt, m_EntityManager.get());
	m_TrackController->Update(dt, m_EntityManager.get(), m_TextureManager.get());
}
