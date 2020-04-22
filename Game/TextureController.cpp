#include "precomp.h"
#include "TextureController.h"

bool Nitro::TextureController::Init(Engine::Renderer* renderer_, Engine::TextureManager* textureManager_, std::string texturesRootDir_)
{
	ASSERT(textureManager_ != nullptr, "Must pass a valid texture manager");

	{
		std::string player1TexturePath = texturesRootDir_ + "/player1Texture.svg";
		if(!textureManager_->CreateTexture(renderer_, "player1Texture", player1TexturePath))
		{
			LOG_ERROR("Failed to create player1Texture");
			return false;
		}
	}

	{
		std::string player2TexturePath = texturesRootDir_ + "/player2Texture.svg";
		if (!textureManager_->CreateTexture(renderer_, "player2Texture", player2TexturePath))
		{
			LOG_ERROR("Failed to create player1Texture");
			return false;
		}
	}
	
	return true;
}
