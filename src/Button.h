#pragma once

#include <SDL2/SDL.h>
#include <string>

enum class ButtonState {
    MouseOut = 0,
    MouseOverMotion = 1,
    MouseDown = 2,
    MouseUp = 3,
};

class Button {
private:
    ButtonState currentState;
    SDL_Texture* texture; // lưu trữ texture của button (có thể là chữ, icon, hình ảnh...)

    SDL_Rect buttonRect;
    SDL_Rect textureRect;

    bool selected;

protected:
    SDL_Color mouseOutColor;
    SDL_Color mouseOverMotionColor;
    SDL_Color mouseDownColor;
    SDL_Color mouseUpColor;

public:
    Button();

    void setTexture(SDL_Texture* texture);
    void setButtonRect(const SDL_Rect& rect);
    void setTextureRect(const SDL_Rect& rect);
    SDL_Rect getButtonRect() const { return buttonRect; }

    void centerTextureRect();
    void setSelected(bool selected);
    bool isMouseInside(int x, int y) const;
    ButtonState getMouseEvent(const SDL_Event* event);

    void setMouseDownColour(const SDL_Color& colour);
    void setMouseUpColour(const SDL_Color& colour);
    void setMouseOverMotionColour(const SDL_Color& colour);
    void setMouseOutColour(const SDL_Color& colour);

    SDL_Color getMouseDownColor() const;
    SDL_Color getMouseUpColor() const;
    SDL_Color getMouseOverMotionColor() const;
    SDL_Color getMouseOutColor() const;

    void renderButton(SDL_Renderer* renderer);
    void renderTexture(SDL_Renderer* renderer);
};
