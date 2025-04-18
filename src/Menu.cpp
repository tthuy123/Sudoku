#include "Menu.h"
#include "Sudoku.h"
#include <iostream>

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 560;

Menu::Menu() : window(nullptr), renderer(nullptr), font(nullptr) 
{

}

Menu::~Menu() {
    close();
}

bool Menu::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0 || TTF_Init() == -1) {
        std::cerr << "SDL Init Error: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow("Menu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) return false;

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) return false;

    font = TTF_OpenFont("assets/Lexend-SemiBold.ttf", 24);
    if (!font) return false;

    SDL_Rect positions[3] = {
        {140, 450, 100, 50},
        {450, 450, 100, 50},
        {750, 450, 100, 50}
    };
    const char* labels[3] = {"start", "level", "quit"};

    for (int i = 0; i < 3; ++i) {
        buttons[i].setButtonRect(positions[i]);
        SDL_Surface* surface = TTF_RenderText_Solid(font, labels[i], {0, 0, 0, 255});
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        buttons[i].setText(labels[i]);
        buttons[i].setTexture(texture);
        buttons[i].centerTextureRect();
        SDL_FreeSurface(surface);
        buttons[i].setMouseOutColour({255,246,247,255});
    }

        SDL_Surface* bgSurface = IMG_Load("assets/background.jpg");
    if (!bgSurface) {
        std::cerr << "Failed to load background image: " << IMG_GetError() << std::endl;
    } else {
        backgroundTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
        SDL_FreeSurface(bgSurface);
    }

    return true;
}

void Menu::renderText(const std::string& text, int x, int y, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

std::string Menu::difficultyToString(Sudoku::Difficulty level) {
    switch (level) {
        case Sudoku::Difficulty::EASY: return "EASY";
        case Sudoku::Difficulty::MEDIUM: return "MEDIUM";
        case Sudoku::Difficulty::HARD: return "HARD";
        default: return "UNKNOWN";
    }
}

void Menu::showLevelMenu() {
    int startX = SCREEN_WIDTH / 2 - 75;
    SDL_Rect levelRects[3] = {
        {startX, 100, 150, 50},
        {startX, 180, 150, 50},
        {startX, 260, 150, 50}
    };
    const char* levels[3] = {"EASY", "MEDIUM", "HARD"};

    Button levelButtons[3];
    for (int i = 0; i < 3; ++i) {
        levelButtons[i].setButtonRect(levelRects[i]);
        SDL_Surface* surface = TTF_RenderText_Solid(font, levels[i], {0, 0, 0, 255});
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        levelButtons[i].setText(levels[i]);
        levelButtons[i].setTexture(texture);
        levelButtons[i].centerTextureRect();
        SDL_FreeSurface(surface);
        levelButtons[i].setMouseOutColour({173, 216, 230, 255});
    }

    bool choosing = true;
    while (choosing) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                SDL_Quit();
                exit(0);
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x = e.button.x;
                int y = e.button.y;
                for (int i = 0; i < 3; ++i) {
                    if (levelButtons[i].isMouseInside(x, y)) {
                        if (i == 0) selectedDifficulty = Sudoku::Difficulty::EASY;
                        else if (i == 1) selectedDifficulty = Sudoku::Difficulty::MEDIUM;
                        else if (i == 2) selectedDifficulty = Sudoku::Difficulty::HARD;
                        choosing = false;
                    }
                }
            }
            std::cout << "Selected difficulty: " << difficultyToString(selectedDifficulty) << std::endl;
        }

        SDL_SetRenderDrawColor(renderer, 255, 228, 225, 255);
        SDL_RenderClear(renderer);
        renderText("Choose Level", startX, 40, {255, 20, 147, 255});
        for (int i = 0; i < 3; ++i) {
            levelButtons[i].renderButton(renderer);
            levelButtons[i].renderTexture(renderer);
            levelButtons[i].centerTextureRect();
        }
        SDL_RenderPresent(renderer);
    }

    for (int i = 0; i < 3; ++i) {
        SDL_DestroyTexture(levelButtons[i].getTexture());
    }
}


void Menu::run() {
    bool running = true;
    while (running) {
        if (!init()) {
            std::cerr << "Failed to initialize menu!\n";
            return;
        }

        SDL_Event event;
        bool startSudoku = false;

        while (running && !startSudoku) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) running = false;
                else if (event.type == SDL_MOUSEBUTTONDOWN) {
                    int x = event.button.x, y = event.button.y;
                    for (int i = 0; i < 3; i++) {
                        if (buttons[i].isMouseInside(x, y)) {
                            std::string label = buttons[i].getText();
                            if (label == "start") {
                                startSudoku = true;
                            } else if (label == "level") {
                                showLevelMenu();
                            } else if (label == "quit") {
                                running = false;
                                SDL_Quit();
                                exit(0);
                            }
                        }
                    }
                }
            }

            // SDL_SetRenderDrawColor(renderer, 211, 211, 211, 255);
            // SDL_RenderClear(renderer);
            if (backgroundTexture) {
                SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);
            } else {
                SDL_SetRenderDrawColor(renderer, 211, 211, 211, 255);
                SDL_RenderClear(renderer);
            }            
          //  renderText("Menu", 120, 40, {255, 105, 180, 255});
            for (int i = 0; i < 3; ++i) {
                buttons[i].renderButton(renderer);
                buttons[i].renderTexture(renderer);
                buttons[i].centerTextureRect();
            }
            SDL_RenderPresent(renderer);

        }

        if (startSudoku) {
            close();
            Sudoku::Sudoku sudokuGame;
            sudokuGame.setDifficulty(selectedDifficulty);
            sudokuGame.play();
        }
    }

    close(); 
}

void Menu::close() {
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    if (backgroundTexture) {
        SDL_DestroyTexture(backgroundTexture);
        backgroundTexture = nullptr;
    }    
    TTF_Quit();
    SDL_Quit();
}
