#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <cstdlib>

#include "Button.h"

namespace Sudoku {

class Cell : public Button {
private:
    bool editable;
    char numberChar;
    char solutionChar;

public:
    Cell();
    void setNumber(const int number);
    char getNumber() const;
    void setSolution(int solution);
    char getSolution() const;
    void setEditable(bool editable);
    bool isEditable() const;

 //   void handleKeyboardEvent(const SDL_Event* event, SDL_Texture* textureCache[]);

    bool isCorrect() const;
};

} 
