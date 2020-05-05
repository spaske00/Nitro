#include "precomp.h"

#include "Matrix.h"
#include "TextureManager.h"

#include <SDL_surface.h>

#include "Render/Texture.h"
#include "Render/Renderer.h"

namespace Engine
{

    bool TextureManager::CreateTexture(Renderer* renderer_, std::string name_, std::string path_)
    {
        if (m_Textures.find(name_) != m_Textures.end()) {
            LOG_ERROR("Attempting to create a texture twice! name: {}, path: {}", name_, path_);
            return false;
        }

        m_Textures.emplace(name_, std::make_unique<Texture>(renderer_, path_));

        return m_Textures.at(name_)->m_Texture != nullptr;
    }


	
	bool TextureManager::CreateTextureFromColorTileMatrix(Renderer* renderer_, std::string name_, const Matrix<Color>& colorMap_,
		int tileHeight_, int tileWidth_)
    {
		auto result = Texture::CreateTextureFromColorMatrix(renderer_, colorMap_, tileHeight_, tileWidth_);
		if (!result)
		{
			LOG_ERROR("Failed to create a texture from color tile matrix");
			return false;
			
		}
		
		m_Textures.emplace(std::move(name_), std::move(result));
		return true;
    }
	
	
    Texture* TextureManager::GetTexture(std::string name_)
    {
        if (m_Textures.find(name_) == m_Textures.end()) {
            LOG_ERROR("Attempting to get a texture that doesn't exist! name: {}", name_);
            return nullptr;
        }

        return m_Textures.at(name_).get();
    }

    bool TextureManager::AddTexture(std::string name_, std::unique_ptr<Texture> texture_)
    {
		if (m_Textures.find(name_) != m_Textures.end()) {
			LOG_ERROR("Attempting to create a texture twice! name: {}", name_);
			return false;
		}
		m_Textures[name_] = std::move(texture_);
		return true;
    }
}
