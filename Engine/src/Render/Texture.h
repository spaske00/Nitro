#pragma once


struct SDL_Texture;

namespace Engine {

    class Renderer;
	struct ColorA;
	template<typename T>
	class Matrix;
	
    struct Texture
    {
        SDL_Texture* m_Texture{};

		static std::unique_ptr<Texture>
    	CreateTextureFromColorMatrix(Renderer* renderer_, const Matrix<ColorA>& colorMatrix_, int tileHeight_, int tileWidth_);
        bool LoadTexture(Renderer* renderer_, std::string path_);
        Texture() = default;
        Texture(Renderer* renderer_, std::string path_);
        ~Texture();
    };
}
