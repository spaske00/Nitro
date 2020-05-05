#pragma once

namespace Engine
{

	template<typename T>
	class Matrix
	{
	public:
		using storage_type = std::vector<T>;
		using value_type = T;
		using size_type = int32_t;
		using iterator = typename storage_type::iterator;
		using const_iterator = typename storage_type::const_iterator;

		struct Coords
		{
			size_type r{};
			size_type c{};
		};

	private:
		std::vector<value_type> m_Data;
		size_type m_Rows{};
		size_type m_Cols{};

	public:
		Matrix() = default;
		Matrix(size_type rows_, size_type cols_)
			: m_Data(rows_* cols_)
			, m_Rows(rows_)
			, m_Cols(cols_)
		{}

		size_type Rows() const { return m_Rows; }
		size_type Cols() const { return m_Cols; }

		value_type At(size_type r, size_type c) const
		{
			ASSERT(r < m_Rows && c < m_Cols, "Invalid row column index");
			return m_Data[m_Cols * r + c];
		}

		value_type& At(size_type r, size_type c)
		{
			ASSERT(r < m_Rows && c < m_Cols, "Invalid row column index");
			return m_Data[m_Cols * r + c];
		}

		value_type At(Coords coords) const
		{
			ASSERT(coords.r < m_Rows && coords.c < m_Cols, "Invalid row column index");
			return m_Data[m_Cols * coords.r + coords.c];
		}

		value_type& At(Coords coords)
		{
			ASSERT(coords.r < m_Rows && coords.c < m_Cols, "Invalid row column index");
			return m_Data[m_Cols * coords.r + coords.c];
		}
		
		iterator begin()
		{
			return m_Data.begin();
		}

		iterator end()
		{
			return m_Data.end();
		}

		const_iterator begin() const
		{
			return m_Data.cbegin();
		}

		const_iterator end() const
		{
			return m_Data.cend();
		}

	};

}
