#pragma once
#include <SDL2/SDL.h>
#include <iostream>
#include <stdlib.h>
#include <SDL2/SDL_ttf.h>

#include "Button.h"

namespace Sudoku
{
    class Cell : public Button // class Cell inherit from class Button
    {
        private: 
                // Define editiability of the cell
                bool mEditable;

                // Define number currently in the cell
                char mCharNumber;

                // Define the actual solution of the cell
                char mCharSolution;

        public:
                // Constructor
                Cell();

                // Set and get the number of the cell
                void setNumber(const int number);
                char getNumber() const;

                // Set solution
                void setSolution(const int solution);

                // Set and get the editiability of the cell
                void setEditable(const bool editable);
                bool isEditable() const;

                // Handle events
                void handleKeyboardEvent(const SDL_Event* event, SDL_Texture* textureCache[]);

                // Compare number with solution
                bool isCorrect() const;
    };
};