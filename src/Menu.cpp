#include "Menu.h"
#include "Sudoku.h"
#include <iostream>

const int SCREEN_WIDTH = 300;
const int SCREEN_HEIGHT = 400;

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
        {75, 100, 150, 50},
        {75, 180, 150, 50},
        {75, 260, 150, 50}
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
        buttons[i].setMouseOutColour({255, 182, 193, 255});
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
                                std::cout << "Opening level selection...\n";
                            } else if (label == "quit") {
                                running = false;
                                SDL_Quit();
                                exit(0);
                            }
                        }
                    }
                }
            }

            // Vẽ giao diện menu
            SDL_SetRenderDrawColor(renderer, 211, 211, 211, 255);
            SDL_RenderClear(renderer);
            renderText("Menu", 120, 40, {255, 105, 180, 255});
            for (int i = 0; i < 3; ++i) {
                buttons[i].renderButton(renderer);
                buttons[i].renderTexture(renderer);
            }
            SDL_RenderPresent(renderer);

        }

        if (startSudoku) {
            close();
            Sudoku::Sudoku sudokuGame;
            sudokuGame.play();
        }
    }

    close(); 
}

void Menu::close() {
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}
