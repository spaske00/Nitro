#pragma once


namespace Nitro
{
	class TrackPatternGenerator {
	public:
		enum Type
		{
			H = 0,
			I,
			LeftL,
			RightL,
			LeftBackSlash,
			RightBackSlash,
			TypeCount
		};

		bool Init(int rows = 12, int cols = 7);

		const Engine::Matrix<TileType>& GetNext();
		
		int Rows() const { return m_Rows; }
		int Cols() const { return m_Cols; }

	private:
		std::array<Engine::Matrix<TileType>, static_cast<int>(TypeCount)> m_Patterns;
		Type m_PreviuslyReturned;
		int m_Probabilites[TypeCount][TypeCount] = { 0 };
		int m_Rows;
		int m_Cols;


		void InitProbabilities();
	};

}
