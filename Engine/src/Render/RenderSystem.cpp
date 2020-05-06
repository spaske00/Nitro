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

        if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
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

        m_EntitiesBuffer.clear();
        entityManager->GetAllEntitiesWithComponents<CameraComponent, TransformComponent>(std::back_inserter(m_EntitiesBuffer));
        ASSERT(!m_EntitiesBuffer.empty(), "Must have at least one camera");

        auto camera = m_EntitiesBuffer[0];

        // draw background
        {
            m_EntitiesBuffer.clear();
            entityManager->GetAllEntitiesWithComponent<BackgroundComponent>(std::back_inserter(m_EntitiesBuffer));
            m_Renderer->DrawEntities(m_EntitiesBuffer, camera);
        }

        // draw entities
        {
            m_EntitiesBuffer.clear();
            // So the ones on top end up last 
            
            entityManager->GetAllEntitiesWithComponent<DrawableEntity>(std::back_inserter(m_EntitiesBuffer));
            std::partition(m_EntitiesBuffer.begin(), m_EntitiesBuffer.end(), [](auto* e) {
                return e->GetComponent<Engine::SpriteComponent>()->m_RenderPriority == RenderPriorty::Normal;
             });
            m_Renderer->DrawEntities(m_EntitiesBuffer, camera);

        }
        // show text
        {
            m_EntitiesBuffer.clear();
            entityManager->GetAllEntitiesWithComponent<TextComponent>(std::back_inserter(m_EntitiesBuffer));
            m_Renderer->ShowTexts(m_EntitiesBuffer, camera);

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
