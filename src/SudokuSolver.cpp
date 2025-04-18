#include "SudokuSolver.h"

Sudoku::Solver::Solver()
	: grid(nullptr), genModifier(false), numToIgnore({ 0, 0, 0 })
{

}

inline void Sudoku::Solver::setElement(const int row, const int col, const int num)
{
	grid[row * SIZE + col] = num;
}

inline int Sudoku::Solver::getElement(const int row, const int col) const
{
	return grid[row * SIZE + col];
}

bool Sudoku::Solver::checkValid(const int inputRow, const int inputCol, const int num) const
{

	// check row
	for (int col = 0; col < SIZE; col++)
	{
		if (getElement(inputRow, col) == num)
		{
			return false;
		}
	}

	// check col
	for (int row = 0; row < SIZE; row++)
	{
		if (getElement(row, inputCol) == num)
		{
			return false;
		}
	}

	// check block
	const int startRow = (int)(inputRow / 3) * 3;
	const int startCol = (int)(inputCol / 3) * 3;

	const int endRow = startRow + 3;
	const int endCol = startCol + 3;

	for (int row = startRow; row < endRow; row++)
	{
		for (int col = startCol; col < endCol; col++)
		{
			if (getElement(row, col) == num)
			{
				return false;
			}
		}
	}
	return true;
}
void Sudoku::Solver::setGenModifier(const bool input)
{
	genModifier = input;
}

void Sudoku::Solver::setGrid(int* grid, const Ignore& ignoreInput)
{
	numToIgnore = ignoreInput;
	this->grid = grid;
}

bool Sudoku::Solver::solve()
{
	for (int row = 0; row < SIZE; row++)
	{
		for (int col = 0; col < SIZE; col++)
		{
			if (getElement(row, col) == 0)
			{
				for (int num = 1; num < 10; num++)
				{
					if (!(genModifier && numToIgnore.row == row && numToIgnore.col == col && numToIgnore.num == num))
					{
						if (checkValid(row, col, num))
						{
							setElement(row, col, num);

							if (solve())
							{
								return true;
							}
							setElement(row, col, 0);
						}
					}
				}
				return false;
			}
		}
	}
	return true;
}

void Sudoku::Solver::display() const
{
	for (int row = 0; row < SIZE; row++)
	{
		for (int col = 0; col < SIZE; col++)
		{
			std::cout << grid[row * SIZE + col] << ", ";
		}
		std::cout << std::endl;
	}
}