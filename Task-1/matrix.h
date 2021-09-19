#pragma once
#include <iostream>
#include <string>

struct MatrixException
{
	std::string msg;
	MatrixException(std::string msg) : msg(msg) {};
};

template <typename value_type>
class Matrix
{
private:
	size_t height, width;
	value_type** matrix;

	class MatrixRow
	{
	private:
		value_type* row_ptr;
		size_t width;
	public:
		MatrixRow(value_type* row_ptr, size_t width) : row_ptr(row_ptr), width(width) {};

		value_type& operator[](size_t index) const
		{
			if (index < 0 || index >= this->width)
			{
				throw MatrixException("MatrixException: index of element in a row is inaccessible");
			}
			else
			{
				return this->row_ptr[index];
			}
		}
	};

public:


	Matrix(size_t matr_height, size_t matr_width, value_type default_value = 0)
	{
		if (matr_height <= 0 || matr_width <= 0)
		{
			this->height = 0;
			this->width = 0;

			this->matrix = nullptr;
		}
		else
		{
			this->height = matr_height;
			this->width = matr_width;

			this->matrix = new value_type * [matr_height];

			for (size_t i = 0; i < matr_height; ++i)
			{
				this->matrix[i] = new value_type[matr_width];

				for (size_t j = 0; j < matr_width; ++j)
				{
					this->matrix[i][j] = default_value;
				}
			}
		}
	}


	Matrix(const Matrix& old) :
		height(old.height), width(old.width)
	{
		if (this->height <= 0 || this->width <= 0)
		{
			this->matrix = nullptr;
		}
		else
		{
			this->matrix = new value_type * [height];

			for (size_t i = 0; i < this->height; ++i)
			{
				this->matrix[i] = new value_type[width];
				for (size_t j = 0; j < this->width; ++j)
				{
					this->matrix[i][j] = old.matrix[i][j];
				}
			}
		}
	}

	Matrix& operator=(const Matrix& old)
	{
		if (this == &old)
		{
			return *this;
		}

		if (matrix != nullptr)
		{
			for (size_t i = 0; i < height; ++i)
			{
				delete[] matrix[i];
			}

			delete[] matrix;
		}

		this->height = old.height;
		this->width = old.width;

		if (this->height <= 0 || this->width <= 0)
		{
			this->matrix = nullptr;
		}
		else
		{
			this->matrix = new value_type * [this->height];

			for (size_t i = 0; i < this->height; ++i)
			{
				this->matrix[i] = new value_type[this->width];
				for (size_t j = 0; j < this->width; ++j)
				{
					this->matrix[i][j] = old.matrix[i][j];
				}
			}
		}

		return *this;
	}


	Matrix(const std::initializer_list<std::initializer_list<value_type>>& matr)
	{
		height = matr.size();
		width = matr.begin()->size();

		matrix = new value_type * [height];

		size_t i = 0;
		for (auto row : matr)
		{
			matrix[i] = new value_type[width];

			size_t j = 0;
			for (value_type elem : row)
			{
				matrix[i][j] = elem;
				++j;
			}

			++i;
		}
	}


	~Matrix()
	{
		if (matrix != nullptr)
		{
			for (size_t i = 0; i < height; ++i)
			{
				delete[] matrix[i];
			}

			delete[] matrix;
		}
	}

	size_t get_height() const
	{
		return this->height;
	}

	size_t get_width() const
	{
		return this->width;
	}


	MatrixRow operator[](size_t index) const
	{
		if (index < 0 || index >= this->height)
		{
			throw MatrixException("MatrixException: index of a row is inaccessible");
		}
		else
		{
			return MatrixRow(this->matrix[index], this->width);
		}
	}

	template <typename another_type>
	friend std::istream& operator>>(std::istream&, Matrix<another_type>&);

	template <typename another_type>
	friend std::ostream& operator<<(std::ostream&, const Matrix<another_type>&);

};

template <typename value_type>
std::istream& operator>>(std::istream& is, Matrix<value_type>& matrix)
{
	for (size_t i = 0; i < matrix.height; ++i)
	{
		for (size_t j = 0; j < matrix.width; ++j)
		{
			is >> matrix.matrix[i][j];
		}
	}
	return is;
}

template <typename value_type>
std::ostream& operator<<(std::ostream& os, const Matrix<value_type>& matrix)
{
	for (size_t i = 0; i < matrix.height; ++i)
	{
		for (size_t j = 0; j < matrix.width; ++j)
		{
			os << matrix.matrix[i][j];
			if (j < matrix.width - 1)
				os << ' ';
		}
		if (i < matrix.height - 1)
			os << '\n';
	}
	return os;
}


template <typename value_type>
Matrix<value_type> operator*(const Matrix<value_type>& lhs, const Matrix<value_type>& rhs)
{
	size_t result_height = lhs.get_height();
	size_t common_size = lhs.get_width();
	size_t result_width = rhs.get_width();

	if (common_size != rhs.get_height())
	{
		throw MatrixException("");
	}

	Matrix<value_type> result(result_height, result_width);

	for (size_t i = 0; i < result_height; ++i)
	{
		for (size_t j = 0; j < common_size; ++j)
		{
			for (size_t k = 0; k < result_width; ++k)
			{
				result[i][j] += lhs[i][k] * rhs[k][j];
			}
		}
	}

	return result;
}