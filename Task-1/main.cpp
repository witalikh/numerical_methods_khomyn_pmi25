#include "equations_systems.h"

void choices()
{
	std::cout << "Choose an option: \n";
	std::cout << "1. Solve any system of algebraic linear equations.\n";
	std::cout << "2. Decompose any square matrix into L and U.\n";
	std::cout << "3. Solve an system\n\n x1 + x2 - x3 = 0\n 2x1 - 4x2 + x3 = 3\n x1 - x2 - x3 = -4\n\n";
	std::cout << "4. Decompose a square matrix\n\n 2 -3 2\n -6 6 5 \n 2 6 -5\n\n";
	std::cout << "5. Exit\n";
}

void cin_valid_dimensions(size_t* var)
{
	while (true)
	{
		signed int dim;
		std::cin >> dim;

		if (dim <= 0)
		{
			std::cout << "This is invalid number. Try once more.\n";
		}
		else
		{
			*var = dim;
			return;
		}
	}
}

void query_1()
{
	std::cout << "Enter the number of algebraic linear equations of system: ";
	size_t eqs;
	cin_valid_dimensions(&eqs);

	std::cout << "\nEnter the number of variables: ";
	size_t vars;
	cin_valid_dimensions(&vars);

	std::cout << "Enter the expanded matrix of coefficients: \n";
	Matrix<double> matrix(eqs, vars + 1, 0.);
	std::cin >> matrix;
	
	try
	{
		std::cout << "\nThat's how it looks in triangle matrix form:\n" << gaussian_elimination(matrix) << "\n\n\n";
		auto solution = SLAE_solution(matrix);
		std::cout << "\nThe result of evaluation is the vector [";
		for (size_t i = 0; i < vars; ++i)
			std::cout << "x" << i << " ";
		std::cout << "\b] = [" << solution;
		std::cout << "]\n\n";
	}
	catch (AlgebraException al)
	{
		std::cout << "No solutions found, oops!\n";
	}
}


void query_2()
{
	std::cout << "Enter the dimension number of square_matrix: ";
	size_t dims;
	cin_valid_dimensions(&dims);

	std::cout << "Enter the square matrix (desired without zeros on the diagonals): \n";
	Matrix<double> matrix(dims, dims, 0.);
	std::cin >> matrix;

	try
	{
		auto LU = LU_decomposition(matrix);
		std::cout << "\nThe result of evaluation are the matrices \n\nL = \n";
		std::cout << LU.first << "\n";
		std::cout << "\nU = \n" << LU.second << "\n\n";
	}
	catch (AlgebraException al)
	{
		std::cout << "No decomposition way found, oops!\n";
	}
}

void query_3()
{
	Matrix<double> matrix = {
		{1, 1, -1, 0},
		{2, -4, 1, 3},
		{1, -1, -1, -4}
	};
	std::cout << "That's how it looks in matrix form:\n" << matrix << "\n\n";
	std::cout << "That's how it looks in triangle matrix form:\n" << gaussian_elimination(matrix) << "\n\n";

	auto solution = SLAE_solution(matrix);
	std::cout << "The result of evaluation is the vector [";
	for (size_t i = 0; i < 3; ++i)
		std::cout << "x" << i << " ";
	std::cout << "\b] = [" << solution;
	std::cout << "]\n\n";
}

void query_4()
{
	Matrix<double> matrix =
	{
		{2, -3, 2},
		{-6, 6, -5},
		{2, 6, -5}
	};

	auto LU = LU_decomposition(matrix);
	std::cout << "The result of evaluation are the matrices \n\nL = \n";
	std::cout << LU.first << "\n";
	std::cout << "\nU = \n" << LU.second << "\n\n";
}

void endquery()
{
	std::cout << "\n---------------------------\n";
}

void menu(int query, bool* if_exit)
{
	switch (query) 
	{
	case 1:
		query_1();
		endquery();
		break;
	case 2:
		query_2();
		endquery();
		break;
	case 3:
		query_3();
		endquery();
		break;
	case 4:
		query_4();
		endquery();
		break;
	case 5:
		std::cout << "Goodbye!\n\n";
		*if_exit = true;
		break;
	default:
		std::cout << "No such choice is available! Try another!\n";
		endquery();
		choices();
		break;
	}
}



int main(int argc, char** argv)
{
	bool if_exit = false;
	choices();
	

	int counter = 0;
	while (!if_exit)
	{
		std::cout << "[" << counter << "]: ";
		int query;
		std::cin >> query;
		menu(query, &if_exit);
		++counter;
	}
	return 0;
}
