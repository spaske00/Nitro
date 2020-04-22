#include "precomp.h"
#include "GameApp.h"

#include <Core/EntryPoint.h>

#include "CameraController.h"
#include "DebugController.h"


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
#if _DEBUG
	m_DebugController = DebugController::Create();
	if (!m_DebugController->Init(m_EntityManager.get(), m_TextureManager.get(), 
	                             &m_WindowData, m_RenderSystem->GetRenderer()))
	{
		LOG_ERROR("Failed to initialize DebugController");
		return false;
	}
#endif
	
	m_CameraController = CameraController::Create();
	if (!m_CameraController->Init(m_EntityManager.get(), &m_WindowData))
	{
		LOG_ERROR("Failed to initialize CameraController");
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
	m_DebugController->Update(dt, m_EntityManager.get());
	m_CameraController->Update(dt, m_EntityManager.get());
}
