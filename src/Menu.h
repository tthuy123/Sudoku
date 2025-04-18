#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <string>
#include "Button.h"
#include "SudokuGenerator.h"

class Menu {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* backgroundTexture = nullptr;
    TTF_Font* font;
    Button buttons[3];
    Sudoku::Difficulty selectedDifficulty = Sudoku::Difficulty::EASY;

public:
    Menu();
    ~Menu();

    bool init();
    void run();
    void close();
    void renderText(const std::string& text, int x, int y, SDL_Color color);
    void showLevelMenu();
    std::string difficultyToString(Sudoku::Difficulty level);
};