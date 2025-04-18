#include "Button.h"

Button::Button()
    : currentState(ButtonState::MouseOut),
      texture(nullptr),
      buttonRect{0, 0, 0, 0},
      textureRect{0, 0, 0, 0},
      selected(false),
      mouseOutColor{255, 179, 198, SDL_ALPHA_OPAQUE},
      mouseOverMotionColor{164, 19, 60, SDL_ALPHA_OPAQUE},
      mouseDownColor{95, 89, 191, SDL_ALPHA_OPAQUE},
      mouseUpColor{89, 13, 34, SDL_ALPHA_OPAQUE}
{}

void Button::setTexture(SDL_Texture* texture) {
    this->texture = texture;
}

void Button::setButtonRect(const SDL_Rect& rect) {
    this->buttonRect = rect;
}

void Button::setTextureRect(const SDL_Rect& rect) {
    this->textureRect = rect;
}
void Button::setText(std::string text) {
    this->text = text;
}
void Button::centerTextureRect() {
    int textureWidth, textureHeight;
    SDL_QueryTexture(texture, nullptr, nullptr, &textureWidth, &textureHeight);

    int x = buttonRect.x + (buttonRect.w - textureWidth) / 2;
    int y = buttonRect.y + (buttonRect.h - textureHeight) / 2;

    textureRect = {x, y, textureWidth, textureHeight};
}

void Button::setSelected(bool selected) {
    this->selected = selected;
}

bool Button::isMouseInside(int x, int y) const {
    return (
        x >= buttonRect.x &&
        x <= buttonRect.x + buttonRect.w &&
        y >= buttonRect.y &&
        y <= buttonRect.y + buttonRect.h
    );
}

ButtonState Button::getMouseEvent(const SDL_Event* event) {
    if (event->type == SDL_MOUSEMOTION || event->type == SDL_MOUSEBUTTONDOWN || event->type == SDL_MOUSEBUTTONUP) {
        int x, y;
        SDL_GetMouseState(&x, &y);

        if (!isMouseInside(x, y)) {
            currentState = ButtonState::MouseOut;
        } else {
            switch (event->type) {
                case SDL_MOUSEMOTION:
                    currentState = ButtonState::MouseOverMotion;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    currentState = ButtonState::MouseDown;
                    break;
                case SDL_MOUSEBUTTONUP:
                    currentState = ButtonState::MouseUp;
                    break;
            }
        }
    }
    return currentState;
}

void Button::setMouseDownColour(const SDL_Color& colour) {
    mouseDownColor = colour;
}
void Button::setMouseUpColour(const SDL_Color& colour) {
    mouseUpColor = colour;
}
void Button::setMouseOutColour(const SDL_Color& colour) {
    mouseOutColor = colour;
}
void Button::setMouseOverMotionColour(const SDL_Color& colour) {
    mouseOverMotionColor = colour;
}
SDL_Color Button::getMouseDownColor() const {
    return mouseDownColor;
}
SDL_Color Button::getMouseUpColor() const {
    return mouseUpColor;
}
SDL_Color Button::getMouseOutColor() const {
    return mouseOutColor;
}
SDL_Color Button::getMouseOverMotionColor() const {
    return mouseOverMotionColor;
}

void Button::renderButton(SDL_Renderer* renderer) {
    if (selected) {
        SDL_SetRenderDrawColor(renderer, mouseDownColor.r, mouseDownColor.g, mouseDownColor.b, mouseDownColor.a);
    } else {
        switch (currentState) {
            case ButtonState::MouseOut:
                SDL_SetRenderDrawColor(renderer, mouseOutColor.r, mouseOutColor.g, mouseOutColor.b, mouseOutColor.a);
                break;
            case ButtonState::MouseOverMotion:
                SDL_SetRenderDrawColor(renderer, mouseOverMotionColor.r, mouseOverMotionColor.g, mouseOverMotionColor.b, mouseOverMotionColor.a);
                break;
            case ButtonState::MouseUp:
                SDL_SetRenderDrawColor(renderer, mouseUpColor.r, mouseUpColor.g, mouseUpColor.b, mouseUpColor.a);
                break;
        }
    }

    SDL_RenderFillRect(renderer, &buttonRect);
}

void Button::renderTexture(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, texture, nullptr, &textureRect);
}
