#pragma once
#include "Render/WindowData.h"
#include <SDL.h>
#include <SDL_ttf.h>
struct SDL_Renderer;
struct SDL_Texture;
struct SDL_RendererInfo;
namespace Engine
{
    struct WindowData;
    class Window;
    class Entity;

    class Renderer
    {
    public:
        bool Init(const WindowData& windowData_ = WindowData());
        bool Shutdown();

        void DrawBackgrounds(const std::vector<Entity*>& backgrounds_, const Entity* camera_);
        void DrawEntities(const std::vector<Entity*>& renderables_, const Entity* camera);
        void DrawBackground(const Entity* background_, const Entity* camera_);
        void DrawEntity(const Entity* r, const Entity* camera);
        void ShowTexts(const std::vector<Entity*>& texts_, const Entity* camera);
        void ShowText(const Entity* text_, const Entity* camera);
        void BeginScene() const;
        void EndScene() const;

		int MaxTextureWidth() const;
		int MaxTextureHeight() const;
    	
    	
        SDL_Renderer* GetNativeRenderer() const { return m_NativeRenderer; }

        void SetBackgroundColor(unsigned char bgR_, unsigned char bgG_, unsigned char bgB_, unsigned char bgA_);
        void SetBackgroundColor(const Color& col_);
        ~Renderer();

    private:
        std::unique_ptr<Window> m_Window;
		SDL_RendererInfo m_RendererInfo;

        SDL_Renderer* m_NativeRenderer{ };
        Color m_BackgroundColor{ };

        // Font and color for text;
        TTF_Font* m_Font;
        SDL_Texture *m_TextDisplayTexture;
        SDL_Color m_textColor;
    };
}
