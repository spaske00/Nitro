#include "precomp.h"

#include "Matrix.h"
#include "Color.h"
#include "Texture.h"
#include "Render/Renderer.h"

#include <SDL.h>
#include <SDL_image.h>


namespace Engine
{
    bool Texture::LoadTexture(Renderer* renderer_, std::string path_)
    {
        if (m_Texture != nullptr)
        {
            LOG_WARNING("Overwriting already loaded texture with: {}", path_);
            SDL_DestroyTexture(m_Texture);
        }

        m_Texture = IMG_LoadTexture(renderer_->GetNativeRenderer(), path_.c_str());

        if (m_Texture == nullptr)
        {
            LOG_ERROR("Unable to load texture: {}, SDL_Image returned error {}", path_, IMG_GetError());
            return false;
        }

        return true;
    }

	std::unique_ptr<Texture> Texture::CreateTextureFromColorMatrix(Renderer* renderer_, const Matrix<ColorA>& colorMatrix_, int tileHeight_, int tileWidth_)
    {

		const int rows = (int)colorMatrix_.Rows();
		const int cols = (int)colorMatrix_.Cols();
    	
		const int height = rows * tileHeight_;
		const int width = cols * tileWidth_;
		ASSERT(height > 0, "Height must greater then 0");
		ASSERT(width > 0, "Width must be greater then 0");
    	
		
		SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA32);
		if (!surface)
		{
			LOG_ERROR("Failed to create surface {}", SDL_GetError());
			return nullptr;
		}
		
    	
    	for (int x = 0; x < rows; ++x)
    	{
    		for (int y = 0; y < cols; ++y)
    		{
				SDL_Rect dst{ x * tileWidth_, y * tileHeight_, tileWidth_, tileHeight_ };
				const ColorA color = colorMatrix_.At(x, y);
				SDL_FillRect(surface, &dst, SDL_MapRGBA(surface->format, color.r , color.g, color.b, color.a));
    		}
    	}

		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer_->GetNativeRenderer(), surface);
		ASSERT(texture != nullptr, "Failed to create texture");
    	
		auto result = std::make_unique<Texture>();
		result->m_Texture = texture; // take ownership of the texture
		texture = nullptr;

		// TODO: Should the surface be freed here?
		SDL_FreeSurface(surface);
		return result;
    }

	
    Texture::Texture(Renderer* renderer_, std::string path_)
    {
        LoadTexture(renderer_, path_);
    }

    Texture::~Texture()
    {
        if (m_Texture != nullptr)
        {
            SDL_DestroyTexture(m_Texture);
        }
    }
}
