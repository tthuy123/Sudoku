#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

// Cấu trúc cho Button
struct menuButton {
    SDL_Rect rect;
    SDL_Color color;
    std::string text;
};

class Menu {
public:
    Menu();
    ~Menu();

    bool init(); // Khởi tạo SDL
    void run();  // Vòng lặp chính
    void close(); // Giải phóng tài nguyên

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;
    menuButton buttons[3];

    void renderButton(menuButton& button);
    void renderText(const std::string& text, int x, int y, SDL_Color color);
    void handleClick(int x, int y, bool &running);
};

#endif
