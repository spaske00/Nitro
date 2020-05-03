#pragma once


struct SDL_Texture;

namespace Engine {

    class Renderer;
	template<typename T>
	class Matrix;
	
    struct Texture
    {
    	struct Dimensions
    	{
			int width;
			int height;
    	};
        SDL_Texture* m_Texture{};

		Dimensions QueryDimensions() const;
		
		static std::unique_ptr<Texture>
    	CreateTextureFromColorMatrix(Renderer* renderer_, const Matrix<Color>& colorMatrix_, int tileHeight_, int tileWidth_);

		static Matrix<std::unique_ptr<Texture>>
			CreateMatrixOfTexturesFromMatrixOfColors(Renderer* renderer_, const Matrix<Color>& colorMatrix, int tileHeightInPixels_, int tileWidthInPixels_);
			
    	
        bool LoadTexture(Renderer* renderer_, std::string path_);
        Texture() = default;
        Texture(Renderer* renderer_, std::string path_);
        ~Texture();
    };
}
