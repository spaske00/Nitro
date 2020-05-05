#include "precomp.h"

#include <SDL.h>
#include <SDL_image.h>

#include "Matrix.h"

#include "Texture.h"
#include "Render/Renderer.h"




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


    Texture::Dimensions Texture::QueryDimensions() const
    {
		Dimensions result{-1, -1};
		if (!m_Texture || SDL_QueryTexture(m_Texture, NULL, NULL, &result.width, &result.height) != 0)
		{
			result.height = -1;
			result.width = -1;
		}
		return result;
    }

	std::unique_ptr<Texture> Texture::CreateTextureFromColorMatrix(Renderer* renderer_, const Matrix<Color>& colorMatrix_, int tileHeight_, int tileWidth_)
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
				const Color color = colorMatrix_.At(x, y);
				SDL_FillRect(surface, &dst, SDL_MapRGBA(surface->format, color.r , color.g, color.b, color.a));
    		}
    	}

		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer_->GetNativeRenderer(), surface);
    	if(!texture)
    	{
			LOG_ERROR("Failed to create texture {}", SDL_GetError());
			return nullptr;
    	}
		ASSERT(texture != nullptr, "Failed to create texture");
    	
		auto result = std::make_unique<Texture>();
		result->m_Texture = texture; // take ownership of the texture
		texture = nullptr;

		// TODO: Should the surface be freed here?
		SDL_FreeSurface(surface);
		return result;
    }

	Matrix<std::unique_ptr<Texture>> Texture::CreateMatrixOfTexturesFromMatrixOfColors(Renderer* renderer_, const Matrix<Color>& colorMatrix, int tileHeightInPixels_, int tileWidthInPixels_)
    {
    	// NOTE: ALL dimensions are in PIXELS!
		int maxTextureHeightInPixels = renderer_->MaxTextureHeight(); // 2
		int maxTextureWidthInPixels = renderer_->MaxTextureWidth(); // 2

		int targetTexutreHeightInPixels = (int)colorMatrix.Rows() * tileHeightInPixels_; // 7 
		int targetTextureWidthInPixels = (int)colorMatrix.Cols() * tileWidthInPixels_; // 5

		
		int numberOfColsTextures = (targetTextureWidthInPixels / maxTextureWidthInPixels) + (targetTextureWidthInPixels % maxTextureWidthInPixels != 0); // 3
		int numberOfRowsTextures = (targetTexutreHeightInPixels / maxTextureHeightInPixels) + (targetTexutreHeightInPixels % maxTextureHeightInPixels != 0); // 4

    	
		Matrix<std::unique_ptr<Texture>> result(numberOfRowsTextures, numberOfColsTextures);

    
		
		for (int textureI = 0, totalPixelsHeightLeft = targetTexutreHeightInPixels, colorMatrixStartRow = 0;
			textureI < result.Rows();
			++textureI, totalPixelsHeightLeft -= maxTextureHeightInPixels,
			colorMatrixStartRow += maxTextureHeightInPixels / tileHeightInPixels_)
		{	
			for (int textureJ = 0, totalPixelsWidthLeft = targetTextureWidthInPixels, colorMatrixStartCol = 0;
				textureJ < result.Cols();
				++textureJ,totalPixelsWidthLeft -= maxTextureWidthInPixels,
				colorMatrixStartCol += maxTextureWidthInPixels / tileWidthInPixels_)
			{
				int currentTextureHeightInPixels = std::min(maxTextureHeightInPixels, totalPixelsHeightLeft);
				int currentTextureWidthInPixels = std::min(maxTextureWidthInPixels, totalPixelsWidthLeft);

				int colorMatrixEndRow = colorMatrixStartRow + currentTextureHeightInPixels / tileHeightInPixels_;
				int colorMatrixEndCol = colorMatrixStartCol + currentTextureWidthInPixels / tileWidthInPixels_;

				SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, currentTextureWidthInPixels, currentTextureHeightInPixels, 0, SDL_PIXELFORMAT_RGBA32);
				ASSERT(surface != nullptr, SDL_GetError());
				for (int i = colorMatrixStartRow; i < colorMatrixEndRow; ++i)
				{
					for(int j = colorMatrixStartCol; j < colorMatrixEndCol; ++j)
					{
						SDL_Rect dest{(j - colorMatrixStartCol) * tileWidthInPixels_, (i - colorMatrixStartRow) * tileHeightInPixels_,
						tileWidthInPixels_, tileHeightInPixels_};
						auto color = colorMatrix.At(i, j);
						SDL_FillRect(surface, &dest, SDL_MapRGBA(surface->format, color.r, color.g, color.b, color.a));
						
					}
				}
				SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer_->GetNativeRenderer(), surface);
				ASSERT(texture != nullptr, SDL_GetError());
				
				(result.At(textureI, textureJ) = std::make_unique<Texture>())->m_Texture = texture;
				
				SDL_FreeSurface(surface);
			}
		}
    	
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
