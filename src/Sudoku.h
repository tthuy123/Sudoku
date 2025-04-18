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

		int totalTextures;
		SDL_Texture* textureCache[TOTAL_TEXTURE_CACHE];

		// Dữ liệu trò chơi
		Cell grid[TOTAL_CELL];       
		int solution[TOTAL_CELL]; 

		Button checkButton;
		Button newButton;
		Button solutionButton;
		Button hintButton;

		Button numberButtons[9];

		// Trạng thái trò chơi
		int selectedNumber;         
		int selectedCell;
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

		// level
		Difficulty level = Difficulty::EASY;

	private:
		// Các hàm khởi tạo & logic nội bộ
		bool initialiseSDL();
		inline int getIndex(int row, int col) const;
		void loadTexture(SDL_Texture*& texture, const char* text, SDL_Color& fontColour);
		void preloadTextures();
		void createInterfaceLayout();
		void generateSudoku(); 
		void freeTextures(); 
		void handleEvents(SDL_Event& event, bool& quit);
		void updateGameState();
		void renderUI();

	public:
		Sudoku();
		~Sudoku();

		void setDifficulty(Difficulty level);
		void play();
		void close();
	};
};
