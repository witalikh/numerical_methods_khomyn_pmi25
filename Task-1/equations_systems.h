#pragma once

#include "matrix.h"


struct AlgebraException
{
	std::string msg;
	AlgebraException(std::string msg) : msg(msg) {};
};


template <typename value_type>
Matrix<value_type> gaussian_elimination(const Matrix<value_type>& arg)
{
	Matrix<value_type> matrix = arg;

	for (size_t i = 0; i < matrix.get_height(); ++i)
	{
		// if lead element is zero, swap rows
		if (matrix[i][i] == 0)
		{
			// indicator if found some row to swap with
			bool swapped = false;

			// finding if some row can be swapped with
			for (size_t k = i + 1; k < matrix.get_height(); ++k)
			{
				// the row is found
				if (matrix[k][i] != 0)
				{
					for (size_t j = 0; j < matrix.get_width(); ++j)
					{
						std::swap(matrix[k][j], matrix[i][j]);
					}

					swapped = true;
					break;
				}
			}

			// matrix is impossible to triangulize, so let's approximate
			if (swapped == false)
			{
				continue;
			}
		}

		// if iteration is not skipped
		// for each row that is leading
		for (size_t k = i + 1; k < matrix.get_height(); ++k)
		{
			// memo the coefficient of elimination
			value_type eliminated = matrix[k][i];

			// substitute other rows to triangular form
			for (size_t j = 0; j < matrix.get_width(); ++j)
			{
				matrix[k][j] -= matrix[i][j] * eliminated / matrix[i][i];
			}
		}
	}

	// return a ready matrix
	return matrix;
}


template <typename value_type>
std::pair<Matrix<value_type>, Matrix<value_type>> LU_decomposition(const Matrix<value_type>& entry)
{
	// check if matrix is square
	if (entry.get_height() != entry.get_width())
	{
		throw AlgebraException("MatrixDecompositionException: \
		Non-Square matrix cannot be decomposed to L and U uniformly");
	}

	// getting size of input matrix
	size_t size = entry.get_height();

	Matrix<value_type> upper = entry;
	Matrix<value_type> lower(size, size);

	for (size_t k = 1; k < size; ++k)
	{
		// concretizing lower matrix after upper being concretized or assigned from original
		for (size_t i = k - 1; i < size; ++i)
		{
			for (size_t j = i; j < size; ++j)
			{
				if (upper[i][i] == 0)
				{
					throw AlgebraException("MatrixDecompositionException: \
				Some of diagonal elements is equal zero. Consider permutation of rows");
				}
				else
				{
					lower[j][i] = upper[j][i] / upper[i][i];
				}
			}
		}

		// upper matrix being concretized
		for (size_t i = k; i < size; ++i)
		{
			for (size_t j = k - 1; j < size; ++j)
			{
				upper[i][j] = upper[i][j] - lower[i][k - 1] * upper[k - 1][j];
			}
		}
	}

	return { lower, upper };
}


template <typename value_type>
Matrix<value_type> SLAE_solution(const Matrix<value_type>& arg)
{
	if (arg.get_height() == 0 || arg.get_width() < 2)
	{
		throw MatrixException("MatrixException: No such systems are possible");
	}
	else
	{
		size_t variables = arg.get_width() - 1;

		Matrix<value_type> simplified = gaussian_elimination(arg);
		Matrix<value_type> solution(1, variables, value_type());

		bool none_solutions = false;

		size_t k = variables - 1;
		size_t w = variables;

		for (size_t u = simplified.get_height(); u > 0; --u)
		{
			size_t i = u - 1;

			if (simplified[i][k] == 0)
			{
				if (simplified[i][w] == 0)
				{
					continue;
				}
				else
				{
					none_solutions = true;
					break;
				}
			}
			else
			{
				solution[0][k] = simplified[i][w] / simplified[i][k];
				for (size_t j = i; j > 0; --j)
				{
					simplified[j - 1][w] -= simplified[j - 1][k] * solution[0][k];
					simplified[j - 1][k] = 0;
				}

				if (k == 0)
				{
					break;
				}
				else
				{
					--k;
				}
			}
		}

		if (none_solutions == true)
		{
			throw AlgebraException("No solutions for this system");
		}

		return solution;
	}
}