from abc import ABC, abstractmethod

import numpy as np
from copy import deepcopy


class Diverge(Exception):
    """ If method diverges, inform """
    pass


class EquationSolutionStrategy(ABC):
    """
    Abstract base class
    for different linear equations system solution
    """

    # static properties
    precision = 0.0001
    max_iterations = 100

    @abstractmethod
    def solve_system(self, matrix: np.ndarray):
        pass


class JacobiIterations(EquationSolutionStrategy):
    """
    Class for Jacobi method implementation
    """

    def __str__(self):
        return "Jacobi method"

    def solve_system(self, matrix: np.ndarray):
        """

        :param matrix: linear equations system in extended form
        :return: a vector of solutions
        """

        coefficients_part: np.ndarray = matrix[:, : -1]
        nums_part: np.ndarray = matrix[:, -1]

        diagonals_inv: np.ndarray = np.zeros(coefficients_part.shape)

        for i in range(coefficients_part.shape[0]):
            diagonals_inv[i, i] = 1 / coefficients_part[i, i]
            coefficients_part[i, i] = 0

        # x = C * x + d = -D_inv * (L + R) * x + D_inv * b
        multiplier = -diagonals_inv @ coefficients_part
        adder = diagonals_inv @ nums_part

        prev, curr = np.zeros(adder.shape), deepcopy(adder)

        # main iterations
        # stop when || curr - prev || < precision or too much iterations

        diverging_iterations = 0
        approximation = 1

        while approximation >= self.precision:

            approximation = np.linalg.norm(curr - prev)
            prev, curr = curr, multiplier @ curr + adder

            if approximation > 1:
                diverging_iterations += 1

            if diverging_iterations >= self.max_iterations:
                raise Diverge

        return curr


class SeidelIterations(EquationSolutionStrategy):
    """
    Class for Seidel method implementation
    """

    def __str__(self):
        return "Seidel method"

    def solve_system(self, matrix: np.ndarray):
        """

        :param matrix: linear equations system in extended form
        :return: a vector of solutions
        """

        assert matrix.shape[1] - matrix.shape[0] == 1

        coefficients_part: np.ndarray = matrix[:, :-1]
        nums_part: np.ndarray = matrix[:, -1]

        for i in range(coefficients_part.shape[0]):
            nums_part[i] /= coefficients_part[i, i]
            coefficients_part[i, i] = 0

        prev, curr = np.zeros(nums_part.shape), deepcopy(nums_part)

        # main iterations
        # stop when || curr - prev || < precision

        diverging_iterations = 0
        approximation = 1

        while approximation >= self.precision:

            approximation = np.linalg.norm(curr - prev)
            prev, curr = curr, -coefficients_part @ curr + nums_part

            if approximation > 1:
                diverging_iterations += 1

            if diverging_iterations >= self.max_iterations:
                raise Diverge

        return curr


class MainContext:

    strategy: EquationSolutionStrategy = None

    messages = {

        "empty_strategy": "Strategy is not chosen, please do it first!\n",
        "change_strategy": "The new chosen method of system solution is",
        "chosen_strategy": "will be used to solve the system\n",

        "input_variables_num": "Enter variables count: ",
        "wrong_variables_num": "Entry cannot be interpreted as variables count. \n"
                               "Try once more! \n",

        "input_matrix": "Enter extended matrix of linear equations system:",
        "empty_matrix": "Linear equations system is empty, solution is empty!",

        "solution_output": "The solution of system is a (transposed) vector x^T = ",
        "solution_diverges": "The method diverges on this system. Possible reasons: \n"
                             "1. No solutions at all.\n"
                             "2. Method is incompatible with the system."

    }

    @staticmethod
    def input_matrix(m: int, n: int):
        """ """
        matrix = np.zeros((m, n))

        for i in range(m):
            row = list(map(float, input().split()))
            for j in range(n):
                matrix[i, j] = row[j]

        return matrix

    def set_strategy(self, strategy):
        """ """
        self.strategy = strategy()
        print(self.messages["change_strategy"], str(self.strategy), "\n")
        return

    def execute_strategy(self):
        """ """
        if self.strategy is None:
            print(self.messages["empty_strategy"])
            return
        else:
            print(str(self.strategy), self.messages["chosen_strategy"])

        variables_num = input(self.messages["input_variables_num"]).strip()

        if not variables_num.isnumeric():
            print(self.messages["wrong_variables_num"])
            return

        else:
            variables_num = int(variables_num)

        if variables_num == 0:
            print(self.messages["empty_matrix"])

        else:
            print(self.messages["input_matrix"])
            matrix = self.input_matrix(variables_num, variables_num + 1)

            try:
                solution = self.strategy.solve_system(matrix)
                print(self.messages["solution_output"], solution)

            except Diverge:
                print(self.messages["solution_diverges"])
        return


def main():

    context = MainContext()
    queries_counter = 0

    messages = {
        "choices": "   Choose one of available options: \n"
                   "1. Use Jacobi method to solve linear equations. \n"
                   "2. Use Seidel method to solve linear equations. \n"
                   "3. Solve system of linear equations using method above. \n"
                   "4. Exit.\n",

        "wrong_query": "Wrong option is chosen. Try once more!",
        "exit": "Goodbye!"
    }

    print(messages["choices"])
    while True:
        query = input(f"[{queries_counter}]: ").strip()

        if query == "1":
            context.set_strategy(JacobiIterations)

        elif query == "2":
            context.set_strategy(SeidelIterations)

        elif query == "3":
            context.execute_strategy()

        elif query == "4":
            print(messages["exit"])
            return

        else:
            print(messages["wrong_query"])
            print(messages["choices"])

        queries_counter += 1


if __name__ == "__main__":
    main()
