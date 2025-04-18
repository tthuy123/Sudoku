#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>  // Đừng quên include TTF nếu bạn dùng text
#include <iostream>
#include <ctime>

#include "Button.h"
#include "SudokuCell.h"
#include "SudokuGenerator.h"

const int TOTAL_TEXTURE_CACHE = 14;
const int TOTAL_CELL = 81;

namespace Sudoku
{
	class Sudoku
	{
	private:
		// Kích thước cửa sổ và lưới
		const int windowHeight;
		const int windowWidth;

		const int gridHeight;
		const int gridWidth;

		const int gridRows;
		const int gridCols;

		// SDL
		SDL_Window* window;
		SDL_Renderer* renderer;
		TTF_Font* font;
		int fontSize;

		// Cache texture để hiển thị text nhanh hơn
		int totalTextures;
		SDL_Texture* textureCache[TOTAL_TEXTURE_CACHE];

		// Dữ liệu trò chơi
		Cell grid[TOTAL_CELL];       // Lưới ô Sudoku
		int solution[TOTAL_CELL];    // Lưu nghiệm của bàn Sudoku đã sinh

		// Giao diện người dùng
		Button checkButton;
		Button newButton;
		Button solutionButton;
		Button hintButton;

		Button numberButtons[9];     // Các nút chọn số (1-9)

		// Trạng thái trò chơi
		int selectedNumber;          // Số đang được chọn để điền
		int selectedCell;            // Vị trí ô đang chọn (từ 0 đến 80)
		bool newGameRequested;
		bool checkSolutionRequested;
		bool hintRequested;
		bool completed;

		// Màu nền
		SDL_Color clearColour;

		// Thông báo
		time_t messageStartTime;
    	bool showMessage = false;
   	 	std::string messageText;

	private:
		// Các hàm khởi tạo & logic nội bộ
		bool initialiseSDL();
		inline int getIndex(int row, int col) const;
		void loadTexture(SDL_Texture*& texture, const char* text, SDL_Color& fontColour);
		void preloadTextures(); // tải sẵn các số từ 1-9
		void createInterfaceLayout(); // xác định vị trí các nút
		void generateSudoku(); // sinh mới lưới Sudoku
		void freeTextures(); // giải phóng bộ nhớ
		void handleEvents(SDL_Event& event, bool& quit); // xử lý sự kiện chuột, bàn phím
		void updateGameState(); // cập nhật trạng thái trò chơi
		void renderUI(); // vẽ giao diện và lưới

	public:
		// Khởi tạo và hủy
		Sudoku();
		~Sudoku();

		// Vòng lặp chính
		void play();

		// Đóng game và cleanup
		void close();
	};
};
