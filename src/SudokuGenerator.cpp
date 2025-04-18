#include "SudokuGenerator.h"
#include <iostream>
#include <ctime>
#include <cstdlib>

Sudoku::Generator::Generator() : grid(nullptr), gridSolution(nullptr) {}

inline void Sudoku::Generator::setElement(const int row, const int col, const int num) {
    grid[row * SIZE + col] = num;
}
inline int Sudoku::Generator::getElement(const int row, const int col) const {
    return grid[row * SIZE + col];
}

void Sudoku::Generator::swapNumbers(const int index1, const int index2) {
    if (index1 == index2) return;
    std::swap(grid[index1], grid[index2]);
}
void Sudoku::Generator::swapRows(const int row1, const int row2) {
    for (int col = 0; col < SIZE; ++col) {
        swapNumbers(row1 * SIZE + col, row2 * SIZE + col);
    }
}
void Sudoku::Generator::swapCols(const int col1, const int col2) {
    for (int row = 0; row < SIZE; ++row) {
        swapNumbers(row * SIZE + col1, row * SIZE + col2);
    }
}
void Sudoku::Generator::swapRowBlocks(const int rowBlock1, const int rowBlock2) {
    for (int i = 0; i < 3; ++i) {
        swapRows(rowBlock1 * 3 + i, rowBlock2 * 3 + i);
    }
}
void Sudoku::Generator::swapColBlocks(const int colBlock1, const int colBlock2) {
    for (int i = 0; i < 3; ++i) {
        swapCols(colBlock1 * 3 + i, colBlock2 * 3 + i);
    }
}
void Sudoku::Generator::fillNextRow(const int previousRow, const int nextRow, const int shifts) {
    for (int col = 0; col < SIZE; ++col) {
        int shiftedCol = (col + shifts) % SIZE;
        setElement(nextRow, col, getElement(previousRow, shiftedCol));
    }
}
void Sudoku::Generator::copyGrid(int* targetGrid) const {
    for (int i = 0; i < SIZE * SIZE; ++i) {
        targetGrid[i] = grid[i];
    }
}
void Sudoku::Generator::createCompletedSudoku() {
    srand(static_cast<unsigned int>(time(nullptr)));

    // 1. Tạo hàng đầu tiên: [1..9]
    for (int i = 0; i < SIZE; ++i) {
        grid[i] = i + 1;
    }

    // 2. Xáo trộn hàng đầu tiên
    for (int i = 0; i < 50; ++i) {
        int i1 = rand() % 9;
        int i2 = rand() % 9;
        if (i1 != i2) swapNumbers(i1, i2);
    }

    // 3. Dịch hàng theo mẫu Latin square
    fillNextRow(0, 1, 3);
    fillNextRow(1, 2, 3);
    fillNextRow(2, 3, 1);
    fillNextRow(3, 4, 3);
    fillNextRow(4, 5, 3);
    fillNextRow(5, 6, 1);
    fillNextRow(6, 7, 3);
    fillNextRow(7, 8, 3);

    // 4. Xáo trộn hàng trong từng khối hàng
    for (int block = 0; block < 3; ++block) {
        for (int i = 0; i < 10; ++i) {
            int r1 = block * 3 + rand() % 3;
            int r2 = block * 3 + rand() % 3;
            if (r1 != r2) swapRows(r1, r2);
        }
    }

    // 5. Xáo trộn cột trong từng khối cột
    for (int block = 0; block < 3; ++block) {
        for (int i = 0; i < 10; ++i) {
            int c1 = block * 3 + rand() % 3;
            int c2 = block * 3 + rand() % 3;
            if (c1 != c2) swapCols(c1, c2);
        }
    }

    // 6. Xáo trộn các khối hàng
    for (int i = 0; i < 10; ++i) {
        int b1 = rand() % 3;
        int b2 = rand() % 3;
        if (b1 != b2) swapRowBlocks(b1, b2);
    }

    // 7. Xáo trộn các khối cột
    for (int i = 0; i < 10; ++i) {
        int b1 = rand() % 3;
        int b2 = rand() % 3;
        if (b1 != b2) swapColBlocks(b1, b2);
    }

    // 8. Lưu lời giải vào gridSolution
    for (int i = 0; i < SIZE * SIZE; ++i) {
        gridSolution[i] = grid[i];
    }
}

void Sudoku::Generator::generate(int* grid, int* solutionGrid, Difficulty difficulty) {
    this->grid = grid;
    this->gridSolution = solutionGrid;
    createCompletedSudoku();

    srand(static_cast<unsigned int>(time(nullptr)));

    Solver solver;
    solver.setGenModifier(true); // Bật chế độ cho generator

    bool removed[81] = {};
    int tempGrid[81];
    int toRemove;
    switch (difficulty) {
        case Difficulty::EASY:
            toRemove = 30 + rand() % 10;
            break;
        case Difficulty::MEDIUM:
            toRemove = 50 + rand() % 10;
            break;
        case Difficulty::HARD:
            toRemove = 2; // for testing
            break;
        default:
            toRemove = 30 + rand() % 10; // Mặc định là dễ
            break;
    }

    while (toRemove > 0) {
        int row = rand() % 9;
        int col = rand() % 9;
        int index = row * 9 + col;

        if (!removed[index]) {
            int originalValue = getElement(row, col);

            // Copy lưới hiện tại vào tempGrid và xóa 1 ô
            for (int i = 0; i < 81; ++i) {
                tempGrid[i] = grid[i];
            }
            tempGrid[index] = 0;

            // Gửi ô bị xóa và giá trị bị bỏ qua vào solver
            Ignore ignored = { originalValue, row, col };
            solver.setGrid(tempGrid, ignored);

            // Nếu không giải được thì có thể xóa ô này
            if (!solver.solve()) {
                setElement(row, col, 0);
                removed[index] = true;
                --toRemove;
            }
        }
    }
}

void Sudoku::Generator::display() const {
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            std::cout << getElement(row, col) << ", ";
        }
        std::cout << std::endl;
    }
}
void Sudoku::Generator::displaySolution() const {
	for (int row = 0; row < 9; ++row) {
		for (int col = 0; col < 9; ++col) {
			std::cout << gridSolution[row * SIZE + col] << ", ";
		}
		std::cout << std::endl;
	}
}

