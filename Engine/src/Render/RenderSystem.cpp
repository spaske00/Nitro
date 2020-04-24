#include "precomp.h"

#include <SDL.h>
#include <SDL_image.h>
#include "RenderSystem.h"
#include "Render/Renderer.h"
#include "Render/Window.h"
#include "ECS/EntityManager.h"


namespace Engine
{
    bool RenderSystem::Init(const WindowData& windowData_)
    {
        LOG_INFO("Initializing RenderSystem");

        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            LOG_CRITICAL("Unable to initialize SDL. SDL error: {}", SDL_GetError());
            return false;
        }

        if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) < 0)
        {
            LOG_CRITICAL("Unable to initialize SDL_Image");
            return false;
        }

        m_Renderer = std::make_unique<Renderer>();

        if (!m_Renderer->Init(windowData_))
        {
            LOG_CRITICAL("Unable to initialize renderer");
            return false;
        }

        m_Renderer->Init(windowData_);

		m_SDLBackgroundSurface = SDL_CreateRGBSurface(0, 1000, 5200, 32, 0, 0, 0, 0);


		for (int y = 0; y <= 5000; y += 16)
		{
			for (int x = 0; x <= 1000; x += 16)
			{
				SDL_Rect rect{ x, y, 8, 8 };
				SDL_FillRect(m_SDLBackgroundSurface, &rect, SDL_MapRGBA(m_SDLBackgroundSurface->format, y % 256, y % 256, y % 256, 255));
			}
		}

		m_BackgroundTexture = SDL_CreateTextureFromSurface(m_Renderer->GetNativeRenderer(), m_SDLBackgroundSurface);


    	

        LOG_INFO("RenderSystem initialized successfully");
        return true;
    }

    bool RenderSystem::Shutdown()
    {
        LOG_INFO("Shutting down RenderSystem");

        m_Renderer.reset();
        SDL_Quit();

        return true;
    }

    void RenderSystem::Update(float dt_, EntityManager* entityManager)
    {
        m_Renderer->BeginScene();

        // Get the main camera from the entity manager
        // TODO: Support multiple cameras and switching between them
        auto cameras = entityManager->GetAllEntitiesWithComponents<CameraComponent, TransformComponent>();
        ASSERT(!cameras.empty(), "Must have at least one camera");
		for (auto camera : cameras)
		{
			// Find all entities to draw

			m_Renderer->DrawBackground(m_BackgroundTexture, camera);
			
			auto renderables = entityManager->GetAllEntitiesWithComponents<TransformComponent, SpriteComponent>();
			m_Renderer->DrawEntities(renderables, camera);
		}   

        m_Renderer->EndScene();
    }

    Renderer* RenderSystem::GetRenderer()
    {
        return m_Renderer.get();
    }

    void RenderSystem::SetBackgroundColor(unsigned char bgR_, unsigned char bgG_, unsigned char bgB_, unsigned char bgA_)
    {
        m_Renderer->SetBackgroundColor(bgR_, bgG_, bgB_, bgA_);
    }

    void RenderSystem::SetBackgroundColor(const Color& col_)
    {
        m_Renderer->SetBackgroundColor(col_);
    }

}
