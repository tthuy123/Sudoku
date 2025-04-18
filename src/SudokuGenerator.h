#pragma once
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "SudokuSolver.h"

namespace Sudoku
{
	class Generator
	{
	private:
		int* grid;
		int* gridSolution;

		const int SIZE = 9; 

	private:
		inline void setElement(const int row, const int col, const int num);
		inline int getElement(const int row, const int col) const;
		// Swapping
		void swapNumbers(const int index1, const int index2);
		void swapRows(const int row1, const int row2);
		void swapCols(const int col1, const int col2);
		void swapRowBlocks(const int rowBlock1, const int rowBlock2);
		void swapColBlocks(const int colBlock1, const int colBlock2);
		// Fill next row with previous row with n shifts
		void fillNextRow(const int previousRow, const int nextRow, const int shifts);
		void copyGrid(int* grid) const;
		void createCompletedSudoku();

	public:
		Generator();
		void generate(int* grid, int* solutionGrid);
		void display() const;
		void displaySolution() const;
	};

};