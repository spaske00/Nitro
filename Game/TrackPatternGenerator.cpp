#include "precomp.h"
#include "GameComponents.h"
#include "TrackPatternGenerator.h"

namespace Nitro
{

	void PlaceBeachOnTheSides(Engine::Matrix<TileType>& result)
	{
		for (int i = 0; i < result.Rows(); ++i)
		{
			result.At(i, 0) = TileType::beach_beach_grass;
			result.At(i, result.Cols() - 1) = TileType::grass_beach_beach;
		}
	}

	
	Engine::Matrix<TileType> TileMatrixLeftL(int rows, int cols)
	{
		Engine::Matrix<TileType> result(rows, cols);
		std::fill(std::begin(result), std::end(result), TileType::grass);
		//PlaceBeachOnTheSides(result);
		//std::replace(std::begin(result), std::end(result), TileType::water, TileType::grass);
		for (int i = 0; i < rows; ++i)
		{
			result.At(i, 0) = TileType::road;
			result.At(i, 1) = TileType::road;
		}
		for (int j = 0; j < cols; ++j)
		{
			result.At(rows - 2, j) = TileType::road;
			result.At(rows - 1, j) = TileType::road;
		}
		return result;
	}

	Engine::Matrix<TileType> TileMatrixRightL(int rows, int cols)
	{
		Engine::Matrix<TileType> result(rows, cols);
		std::fill(std::begin(result), std::end(result), TileType::grass);
		//PlaceBeachOnTheSides(result);
		//std::replace(std::begin(result), std::end(result), TileType::water, TileType::grass);

		for (int i = 0; i < rows; ++i)
		{
			result.At(i, cols - 1) = TileType::road;
			result.At(i, cols - 2) = TileType::road;
		}
		for (int j = 0; j < cols; ++j)
		{
			result.At(rows - 2, j) = TileType::road;
			result.At(rows - 1, j) = TileType::road;
		}
		return result;
	}

	Engine::Matrix<TileType> TileMatrixI(int rows, int cols)
	{
		Engine::Matrix<TileType> result(rows, cols);
		std::fill(std::begin(result), std::end(result), TileType::grass);
		//PlaceBeachOnTheSides(result);
		//std::replace(std::begin(result), std::end(result), TileType::water, TileType::grass);

		for (int i = 0; i < cols; ++i)
		{
			result.At(0, i) = TileType::road;
			result.At(1, i) = TileType::road;
			result.At(rows - 2, i) = TileType::road;
			result.At(rows - 1, i) = TileType::road;
		}

		for (int i = 0; i < rows; ++i)
		{
			result.At(i, cols / 2 - 1) = TileType::road;
			result.At(i, cols / 2) = TileType::road;
			result.At(i, cols / 2 + 1) = TileType::road;

		}
		return result;
	}

	Engine::Matrix<TileType> TileMatrixH(int rows, int cols)
	{
		Engine::Matrix<TileType> result(rows, cols);
		std::fill(std::begin(result), std::end(result), TileType::grass);
		//PlaceBeachOnTheSides(result);
		//std::replace(std::begin(result), std::end(result), TileType::water, TileType::grass);

		for (int i = 0; i < cols; ++i)
		{
			result.At(rows /2 - 1, i) = TileType::road;
			result.At(rows /2, i) = TileType::road;
			result.At(rows / 2 + 1, i) = TileType::road;
		}

		for (int i = 0; i < rows; ++i)
		{
			result.At(i, 0) = TileType::road;
			result.At(i, 1) = TileType::road;
			result.At(i, cols - 1) = TileType::road;
			result.At(i, cols - 2) = TileType::road;
		}
		return result;
	}

	Engine::Matrix<TileType> TileMatrixLeftBackSlash(int rows, int cols)
	{
		Engine::Matrix<TileType> result(rows, cols);
		std::fill(std::begin(result), std::end(result), TileType::grass);
		//PlaceBeachOnTheSides(result);
		//std::replace(std::begin(result), std::end(result), TileType::water, TileType::grass);

		for (int i = 0; i < result.Cols(); ++i)
		{
			result.At(std::min(2*i, result.Rows()-1), i) = TileType::road;
			result.At(std::min(2*i+1, result.Rows() - 1), i) = TileType::road;
			result.At(std::min(2*i+2, result.Rows() - 1), i) = TileType::road;
			result.At(std::min(2*i+3, result.Rows() - 1), i) = TileType::road;

		}
		return result;
	}

	Engine::Matrix<TileType> TileMatrixRightBackSlash(int rows, int cols)
	{
		Engine::Matrix<TileType> result(rows, cols);
		std::fill(std::begin(result), std::end(result), TileType::grass);
		//PlaceBeachOnTheSides(result);
		//std::replace(std::begin(result), std::end(result), TileType::water, TileType::grass);

		for (int i = 0; i < result.Cols(); ++i)
		{
			int col = result.Cols() - i - 1;
			result.At(std::min(2 * i, result.Rows() - 1), col) = TileType::road;
			result.At(std::min(2 * i + 1, result.Rows() - 1), col) = TileType::road;
			result.At(std::min(2 * i + 2, result.Rows() - 1), col) = TileType::road;
			result.At(std::min(2 * i + 3, result.Rows() - 1), col) = TileType::road;

		}
		return result;
	}

	bool TrackPatternGenerator::Init(int rows, int cols)
	{
		m_Rows = rows;
		m_Cols = cols;
		m_Patterns[Type::H] = TileMatrixH(rows, cols);
		m_Patterns[Type::I] = TileMatrixI(rows, cols);
		m_Patterns[Type::LeftL] = TileMatrixLeftL(rows, cols);
		m_Patterns[Type::RightL] = TileMatrixRightL(rows, cols);
		m_Patterns[Type::LeftBackSlash] = TileMatrixLeftBackSlash(rows, cols);
		m_Patterns[Type::RightBackSlash] = TileMatrixRightBackSlash(rows, cols);
		// TODO(Marko) :Implement probabilities with Markov Chains

		return true;
	}

	const Engine::Matrix<TileType>& TrackPatternGenerator::GetNext()
	{
		m_PreviuslyReturned = static_cast<Type>((m_PreviuslyReturned + 1) % Type::TypeCount); // shuffle patterns in circle
		ASSERT(m_PreviuslyReturned >= 0 && m_PreviuslyReturned < Type::TypeCount, "Must be in range");
		return m_Patterns[m_PreviuslyReturned];
	}

}