#include "precomp.h"
#include <random>
#include "GameComponents.h"
#include "TrackPatternGenerator.h"

namespace Nitro
{

	void PlaceBeachWithTreesOnTheSides(Engine::Matrix<TileType>& result)
	{
		for (int i = 0; i < result.Rows(); ++i)
		{
			result.At(i, 0) = TileType::beach_trees_grass;
			result.At(i, result.Cols() - 1) = TileType::beach_trees_grass;

		}
	}

	
	Engine::Matrix<TileType> TileMatrixLeftL(int rows, int cols)
	{
		Engine::Matrix<TileType> result(rows, cols);
		std::fill(std::begin(result), std::end(result), TileType::grass);
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
		std::replace(std::begin(result), std::end(result), TileType::grass, TileType::park2);

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
		std::replace(std::begin(result), std::end(result), TileType::grass, TileType::park2);
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
		
		std::replace(std::begin(result), std::end(result), TileType::grass, TileType::park1);

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
		m_PreviuslyReturned = Type::I;
		InitProbabilities();
		return true;
	}

	const Engine::Matrix<TileType>& TrackPatternGenerator::GetNext()
	{
		//m_PreviuslyReturned = static_cast<Type>((m_PreviuslyReturned + 1) % Type::TypeCount); // shuffle patterns in circle
		//ASSERT(m_PreviuslyReturned >= 0 && m_PreviuslyReturned < Type::TypeCount, "Must be in range");
		static std::random_device rd;
		static std::mt19937 gen(rd());
		static std::uniform_int_distribution<> dis(0, 99);

		int sum = 0;
		int target = dis(gen);
		LOG_INFO(fmt::format("Target {}", target));
		LOG_INFO(fmt::format("PRevious {}", m_PreviuslyReturned));
		for (int i = 0; i < Type::TypeCount; ++i)
		{
			sum += m_Probabilites[m_PreviuslyReturned][i];
			if (sum > target)
			{
				m_PreviuslyReturned = static_cast<Type>(i);
				break;
			}
			
		}
		LOG_INFO(fmt::format("Generated Patern: {}", m_PreviuslyReturned));
		return m_Patterns[m_PreviuslyReturned];
	}

	

	void TrackPatternGenerator::InitProbabilities()
	{
		m_Probabilites[Type::H][Type::H] = 16;
		m_Probabilites[Type::H][Type::I] = 20;
		m_Probabilites[Type::H][Type::LeftL] = 16;
		m_Probabilites[Type::H][Type::RightL] = 16;
		m_Probabilites[Type::H][Type::LeftBackSlash] = 16;
		m_Probabilites[Type::H][Type::RightBackSlash] = 16;
		ASSERT(std::accumulate(std::begin(m_Probabilites[Type::H]), std::end(m_Probabilites[Type::H]), 0) == 100, "Sum of ps must be 100");

		m_Probabilites[Type::I][Type::H] = 20;
		m_Probabilites[Type::I][Type::I] = 0;
		m_Probabilites[Type::I][Type::LeftL] = 20;
		m_Probabilites[Type::I][Type::RightL] = 20;
		m_Probabilites[Type::I][Type::LeftBackSlash] = 20;
		m_Probabilites[Type::I][Type::RightBackSlash] = 20;
		ASSERT(std::accumulate(std::begin(m_Probabilites[Type::I]), std::end(m_Probabilites[Type::I]), 0) == 100, "Sum of ps must be 100");


		m_Probabilites[Type::LeftL][Type::H] = 0;
		m_Probabilites[Type::LeftL][Type::I] = 33;
		m_Probabilites[Type::LeftL][Type::LeftL] = 0;
		m_Probabilites[Type::LeftL][Type::RightL] = 33;
		m_Probabilites[Type::LeftL][Type::LeftBackSlash] = 0;
		m_Probabilites[Type::LeftL][Type::RightBackSlash] = 34;
		ASSERT(std::accumulate(std::begin(m_Probabilites[Type::LeftL]), std::end(m_Probabilites[Type::LeftL]), 0) == 100, "Sum of ps must be 100");


		m_Probabilites[Type::RightL][Type::H] = 0;
		m_Probabilites[Type::RightL][Type::I] = 33;
		m_Probabilites[Type::RightL][Type::LeftL] = 33;
		m_Probabilites[Type::RightL][Type::RightL] = 0;
		m_Probabilites[Type::RightL][Type::LeftBackSlash] = 34;
		m_Probabilites[Type::RightL][Type::RightBackSlash] = 0;
		ASSERT(std::accumulate(std::begin(m_Probabilites[Type::RightL]), std::end(m_Probabilites[Type::RightL]), 0) == 100, "Sum of ps must be 100");


		m_Probabilites[Type::LeftBackSlash][Type::H] = 0;
		m_Probabilites[Type::LeftBackSlash][Type::I] = 33; //
		m_Probabilites[Type::LeftBackSlash][Type::LeftL] = 0;
		m_Probabilites[Type::LeftBackSlash][Type::RightL] = 33; //
		m_Probabilites[Type::LeftBackSlash][Type::LeftBackSlash] = 0;
		m_Probabilites[Type::LeftBackSlash][Type::RightBackSlash] = 34; //
		ASSERT(std::accumulate(std::begin(m_Probabilites[Type::LeftBackSlash]), std::end(m_Probabilites[Type::RightBackSlash]), 0) == 100, "Sum of ps must be 100");


		m_Probabilites[Type::RightBackSlash][Type::H] = 0;
		m_Probabilites[Type::RightBackSlash][Type::I] = 33; //
		m_Probabilites[Type::RightBackSlash][Type::LeftL] = 33; //
		m_Probabilites[Type::RightBackSlash][Type::RightL] = 0;
		m_Probabilites[Type::RightBackSlash][Type::LeftBackSlash] = 34; //
		m_Probabilites[Type::RightBackSlash][Type::RightBackSlash] = 0;
		ASSERT(std::accumulate(std::begin(m_Probabilites[Type::RightBackSlash]), std::end(m_Probabilites[Type::RightBackSlash]), 0) == 100, "Sum of ps must be 100");


	}

}