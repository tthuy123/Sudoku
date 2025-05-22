/* Sudoku solver class */
#pragma once
#include <iostream>

namespace Sudoku 
{
	// struct for cell that should be ignore when generate the Sudoku puzzle
	struct Ignore
	{
		int num = 0;
		int row = 0;
		int col = 0;
	};

	class Solver
	{
	private:
		int* grid;

		bool genModifier;
		Ignore numToIgnore;

		const int SIZE = 9;

	private:
		inline void setElement(const int row, const int col, const int num);
		inline int getElement(const int row, const int col) const;

		bool checkValid(const int inputRow, const int inputCol, const int num) const;

	public:
		Solver();

		void setGenModifier(const bool input);

		void setGrid(int* grid, const Ignore& ignoreInput = { 0, 0, 0 });

		bool solve();

		void display() const;

	};

};