#pragma once

namespace Engine {
	class TextureManager;
}

namespace Nitro
{
	class TextureController
	{
	public:
		bool Init(Engine::Renderer* renderer_, Engine::TextureManager* textureManager_, std::string texturesRootDir_);
		static std::unique_ptr<TextureController> Create()
		{
			return std::make_unique<TextureController>();
		}
	};

}

