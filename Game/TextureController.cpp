#include "precomp.h"
#include "TextureController.h"

#include "GameComponents.h"

bool Nitro::TextureController::Init(Engine::Renderer* renderer_, Engine::TextureManager* textureManager_, std::string texturesRootDir_)
{
	ASSERT(textureManager_ != nullptr, "Must pass a valid texture manager");


	if (!textureManager_->CreateTexture(renderer_, "player1Texture", texturesRootDir_ + "/player1Texture.svg"))
	{
		LOG_ERROR("Failed to create player1Texture");
		return false;
	}

	if (!textureManager_->CreateTexture(renderer_, "player2Texture", texturesRootDir_ + "/player2Texture.svg"))
	{
		LOG_ERROR("Failed to create player1Texture");
		return false;
	}

	

	for(int i = 0; i < (int)TileType::TileTypeCount; ++i)
	{
		auto name = TileTypeToTextureName(static_cast<TileType>(i));
		if (!textureManager_->CreateTexture(renderer_, name, texturesRootDir_ + "/" + name + ".png"))
		{
			LOG_ERROR(fmt::format("Failed to load texture {}", name));
		}
		else
		{
			LOG_INFO(fmt::format("Loaded texture: {}", name));
		}
	}

	
	
#if _DEBUG
	if (!textureManager_->CreateTexture(renderer_, "background_debug_texture_grid", texturesRootDir_ + "/background_debug_texture_grid.png"))
	{
		LOG_ERROR("Failed to load bacgkround_Debug_texture_gird");
		return false;
	}
	
	if (!textureManager_->CreateTexture(renderer_, "blank", texturesRootDir_ + "/blank.png"))
	{
		LOG_ERROR("Failed to load blank texture");
		return false;
	}
#endif

	return true;
}


