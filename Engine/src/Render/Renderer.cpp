#include "precomp.h"

#include <SDL.h>

#include "Renderer.h"
#include "Render/Window.h"
#include "Render/Texture.h"
#include "ECS/Entity.h"



namespace Engine
{

	bool Renderer::Init(const WindowData& windowData_)
	{
		LOG_INFO("Initializing Renderer");

		
		m_Window = std::make_unique<Window>();

		if (!m_Window->Init(windowData_))
		{
			LOG_CRITICAL("Unable to create a Window.");
		}

		m_NativeRenderer = SDL_CreateRenderer(
			m_Window->GetNativeWindowHandle(),
			-1,
			windowData_.m_Vsync ? SDL_RENDERER_PRESENTVSYNC : 0 |
			SDL_RENDERER_ACCELERATED);

		if (m_NativeRenderer == nullptr)
		{
			LOG_CRITICAL("Unable to create a renderer. SDL error: {}", SDL_GetError());
			return false;
		}
		
		SetBackgroundColor(100, 150, 236, SDL_ALPHA_OPAQUE);

		
		SDL_GetRendererInfo(m_NativeRenderer, &m_RendererInfo);


		LOG_INFO("RenderSystem initialized successfully");
		
		
		return true;
	}

	bool Renderer::Shutdown()
	{
		LOG_INFO("Shutting down Renderer");

		if (m_NativeRenderer != nullptr)
		{
			SDL_DestroyRenderer(m_NativeRenderer);
		}

		m_NativeRenderer = nullptr;

		m_Window.reset();

		return true;
	}

	vec2 GetScreenPosition(vec2 targetPosition, const Entity* camera)
	{
		vec2 screenCenter{ camera->GetComponent<TransformComponent>()->m_Size / 2.0f };
		return targetPosition - camera->GetComponent<TransformComponent>()->m_Position + screenCenter;
	}

	bool IsInsideScreen(vec2 targetWorldPosition, vec2 targetSize, const Entity* camera)
	{
		vec2 screenPosition = GetScreenPosition(targetWorldPosition, camera);
		return (screenPosition.x + targetSize.x / 2.0f >= 0 && screenPosition.x - targetSize.x / 2.0f <= camera->GetComponent<TransformComponent>()->m_Size.x)
			&& (screenPosition.y + targetSize.y / 2.0f >= 0 && screenPosition.y - targetSize.y / 2.0f <= camera->GetComponent<TransformComponent>()->m_Size.y);
	}

	void Renderer::DrawBackgrounds(const std::vector<Entity*>& backgrounds_, const Entity* camera_)
	{
		for (auto b : backgrounds_)
		{
			DrawBackground(b, camera_);
		}
	}

	void Renderer::DrawEntities(const std::vector<Entity*>& renderables_, const Entity* camera)
	{
		for (const auto r : renderables_)
		{
			DrawEntity(r, camera);
		}
	}

	SDL_Rect CenterPositionToSDL_Rect(vec2 position, vec2 size)
	{
		SDL_Rect result{
			(int)(position.x - size.x / 2), (int)(position.y - size.y / 2), (int)size.x, (int)size.y
		};
		return result;
	}
	
	
	
	void Renderer::DrawBackground(const Entity* background_, const Entity* camera_)
	{
		ASSERT(background_->HasComponent<BackgroundComponent>(), "background must have a background component");
		
		auto backgroundTransform = background_->GetComponent<TransformComponent>();
		auto backgroundSprite = background_->GetComponent<SpriteComponent>();

		
		auto backgroundPosition = backgroundTransform->m_Position;
		auto backgroundRotation = backgroundTransform->m_Rotation;
		auto backgroundSize = backgroundTransform->m_Size;

		auto cameraPosition = camera_->GetComponent<TransformComponent>()->m_Position;
		auto cameraSize = camera_->GetComponent<TransformComponent>()->m_Size;
		auto cameraOnScreenPositionOffset = camera_->GetComponent<CameraComponent>()->m_OnScreenPositionOffset;

		if (IsInsideScreen(backgroundPosition, backgroundSize, camera_))
		{
			vec2 screenPosition = GetScreenPosition(backgroundPosition, camera_) + cameraOnScreenPositionOffset;
			//SDL_Rect src{ (int)cameraPosition.x, (int)abs(cameraPosition.y), (int)cameraSize.x, (int)cameraSize.y };

			SDL_Rect texture = CenterPositionToSDL_Rect(backgroundPosition, backgroundSize);
			SDL_Rect camera = CenterPositionToSDL_Rect(cameraPosition, cameraSize);

			SDL_Rect result;
			if (!SDL_IntersectRect(&texture, &camera, &result))
			{
				
				return;
			}

			SDL_Rect src = result;
			src.x -= texture.x;
			src.y -= texture.y;

			SDL_Rect dst = result;
			dst.x -= camera.x;
			dst.y -= camera.y;
			dst.x += (int)cameraOnScreenPositionOffset.x;
			dst.y += (int)cameraOnScreenPositionOffset.y;
			
			
			ASSERT(src.x >= 0 && src.y >= 0, "Must be greater then 0");
			
			//SDL_Rect dst{ (int)(screenPosition.x - backgroundSize.x / 2), (int)(screenPosition.y - backgroundSize.y / 2), (int)backgroundSize.x, (int)backgroundSize.y };
			
			//SDL_Rect dst{ (int)cameraOnScreenPositionOffset.x + dest.x, (int)cameraOnScreenPositionOffset.y, + dest.x, (int)cameraSize.x, (int)cameraSize.y };
			SDL_RendererFlip flip = static_cast<SDL_RendererFlip>((backgroundSprite->m_FlipHorizontal ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE) | (backgroundSprite->m_FlipVertical ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE));

#if 0
			SDL_Rect src;
			SDL_Rect dst{ (int)(screenPosition.x - backgroundSize.x / 2), (int)(screenPosition.y - backgroundSize.y / 2), (int)backgroundSize.x, (int)backgroundSize.y };

#endif
			SDL_RenderCopyEx(
				m_NativeRenderer,
				backgroundSprite->m_Image->m_Texture,
				&src,
				&dst,
				backgroundTransform->m_Rotation,
				NULL,
				flip
			);
		}
		
#if 0
		auto transform = camera_->GetComponent<TransformComponent>();
		
		SDL_Rect src{ (int)transform->m_Position.x, (int)abs(transform->m_Position.y), (int)transform->m_Size.x, (int)transform->m_Size.y };
		vec2 screenPosition = camera_->GetComponent<CameraComponent>()->m_OnScreenPositionOffset;
		SDL_Rect dst{ (int)(screenPosition.x), (int)(screenPosition.y), (int)transform->m_Size.x, (int)transform->m_Size.y };
		SDL_RenderCopyEx(m_NativeRenderer, backgroundTexture_, &src, &dst, 0.f, NULL, static_cast<SDL_RendererFlip>(SDL_FLIP_NONE));
#endif
	}
	

	void Renderer::DrawEntity(const Entity* r, const Entity* camera)
	{

		auto transform = r->GetComponent<TransformComponent>();
		auto sprite = r->GetComponent<SpriteComponent>();

		vec2 size = transform->m_Size;
		if (size == vec2{ 0.f, 0.f }) // Use size of texture if size of entity isn't set
		{
			int w, h;
			SDL_QueryTexture(sprite->m_Image->m_Texture, NULL, NULL, &w, &h);
			size.x = static_cast<float>(w);
			size.y = static_cast<float>(h);
		}

		if (IsInsideScreen(transform->m_Position, vec2(size.x, size.y), camera))
		{
			vec2 screenPosition = GetScreenPosition(transform->m_Position, camera) + camera->GetComponent<CameraComponent>()->m_OnScreenPositionOffset;
			SDL_Rect dst{ (int)(screenPosition.x - size.x / 2), (int)(screenPosition.y - size.y / 2), (int)size.x, (int)size.y };
			SDL_RendererFlip flip = static_cast<SDL_RendererFlip>((sprite->m_FlipHorizontal ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE) | (sprite->m_FlipVertical ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE));

			SDL_RenderCopyEx(
				m_NativeRenderer,
				sprite->m_Image->m_Texture,
				NULL,
				&dst,
				transform->m_Rotation,
				NULL,
				flip);

#ifdef _DEBUG
			// DebugDraw
			SDL_SetRenderDrawColor(m_NativeRenderer, 255, 0, 0, SDL_ALPHA_OPAQUE);

			SDL_RenderDrawPoint(m_NativeRenderer, (int)screenPosition.x, (int)screenPosition.y);

			if (auto collider = r->GetComponent<CollisionComponent>())
			{
				SDL_SetRenderDrawColor(m_NativeRenderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
				dst = { (int)(screenPosition.x - collider->m_Size.x / 2), (int)(screenPosition.y - collider->m_Size.y / 2), (int)collider->m_Size.x, (int)collider->m_Size.y };
				SDL_RenderDrawRect(m_NativeRenderer, &dst);

			}

			SetBackgroundColor(m_BackgroundColor);
#endif

		}


	}

	void Renderer::SetBackgroundColor(unsigned char bgR_, unsigned char bgG_, unsigned char bgB_, unsigned char bgA_)
	{
		m_BackgroundColor.r = bgR_;
		m_BackgroundColor.g = bgG_;
		m_BackgroundColor.b = bgB_;
		m_BackgroundColor.a = bgA_;
		SDL_SetRenderDrawColor(m_NativeRenderer, bgR_, bgG_, bgB_, bgA_);
	}

	void Renderer::SetBackgroundColor(const Color& col_)
	{
		m_BackgroundColor = col_;
		SDL_SetRenderDrawColor(m_NativeRenderer, m_BackgroundColor.r, m_BackgroundColor.g, m_BackgroundColor.b, m_BackgroundColor.a);
	}

	void Renderer::BeginScene() const
	{
		SDL_RenderClear(m_NativeRenderer);
	}

	void Renderer::EndScene() const
	{
		SDL_RenderPresent(m_NativeRenderer);
	}

	int Renderer::MaxTextureWidth() const
	{
		return m_RendererInfo.max_texture_width;
	}

	int Renderer::MaxTextureHeight() const
	{
		return m_RendererInfo.max_texture_height;
		
	}

	Renderer::~Renderer()
	{
		Shutdown();
	}

}
