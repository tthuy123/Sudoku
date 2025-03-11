#include "Menu.h"
#include "Sudoku.h"
#include <iostream>

const int SCREEN_WIDTH = 300;
const int SCREEN_HEIGHT = 400;

Menu::Menu() : window(nullptr), renderer(nullptr), font(nullptr) {}

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

    font = TTF_OpenFont("assets/inter_bold.ttf", 24);
    if (!font) return false;

    // Tạo các button
    buttons[0] = {{75, 100, 150, 50}, {255, 182, 193, 255}, "start"};
    buttons[1] = {{75, 180, 150, 50}, {255, 182, 193, 255}, "level"};
    buttons[2] = {{75, 260, 150, 50}, {255, 182, 193, 255}, "quit"};

    return true;
}

void Menu::renderButton(menuButton& button) {
    SDL_SetRenderDrawColor(renderer, button.color.r, button.color.g, button.color.b, 255);
    SDL_RenderFillRect(renderer, &button.rect);
    renderText(button.text, button.rect.x + 50, button.rect.y + 15, {139, 0, 0, 255});
}

void Menu::renderText(const std::string& text, int x, int y, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void Menu::handleClick(int x, int y, bool &running) { 
    for (int i = 0; i < 3; i++) {
        if (x > buttons[i].rect.x && x < buttons[i].rect.x + buttons[i].rect.w &&
            y > buttons[i].rect.y && y < buttons[i].rect.y + buttons[i].rect.h) {
            
            if (buttons[i].text == "start") {
                running = false; 
            } 
            else if (buttons[i].text == "level") {
                std::cout << "Opening level selection...\n";
            } 
            else if (buttons[i].text == "quit") {
                running = false;
                SDL_Quit();
                exit(0);
            }
        }
    }
}


void Menu::run() {
    bool running = true;
    while (running) {
        // Khởi tạo menu
        if (!init()) {
            std::cerr << "Failed to initialize menu!\n";
            return;
        }

        SDL_Event event;
        bool startSudoku = false; // Cờ kiểm tra xem có mở Sudoku không

        while (running && !startSudoku) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) running = false;
                else if (event.type == SDL_MOUSEBUTTONDOWN) {
                    int x = event.button.x, y = event.button.y;
                    for (int i = 0; i < 3; i++) {
                        if (x > buttons[i].rect.x && x < buttons[i].rect.x + buttons[i].rect.w &&
                            y > buttons[i].rect.y && y < buttons[i].rect.y + buttons[i].rect.h) {
                            
                            if (buttons[i].text == "start") {
                                startSudoku = true; // Đánh dấu để chuyển sang Sudoku
                            } 
                            else if (buttons[i].text == "level") {
                                std::cout << "Opening level selection...\n";
                            } 
                            else if (buttons[i].text == "quit") {
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
            for (int i = 0; i < 3; i++) renderButton(buttons[i]);
            SDL_RenderPresent(renderer);
        }

        // Nếu người dùng chọn "start", đóng menu và mở Sudoku
        if (startSudoku) {
            close();  // Đóng menu trước khi mở Sudoku
            Sudoku::Sudoku sudokuGame;
            sudokuGame.play();
        }
    }

    close(); // Đóng ứng dụng khi thoát khỏi vòng lặp
}

void Menu::close() {
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}
