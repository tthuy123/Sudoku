#include <cctype>  
#include <cstring>  
#include "SudokuCell.h"


Sudoku::Cell::Cell():
    editable(false), numberChar(' '), solutionChar(' ')
{
}

void Sudoku::Cell::setNumber(const int number)
{
    if (number == 0)
    {
        numberChar = ' ';
    }
    else
    {
        numberChar = static_cast<char>(number + '0');
    }
}

char Sudoku::Cell::getNumber() const
{
    return numberChar;
}

void Sudoku::Cell::setSolution(const int solution)
{
    if (solution == 0)
    {
        solutionChar = ' ';
    }
    else
    {
        solutionChar = static_cast<char>(solution + '0');
    }
}

void Sudoku::Cell::setEditable(const bool editable)
{
    this->editable = editable;
    if (this->editable)
    {
        mouseOutColor = { 255, 179, 193, SDL_ALPHA_OPAQUE }; 
        mouseOverMotionColor = { 95, 89, 191, SDL_ALPHA_OPAQUE }; 
        mouseDownColor = { 201, 24, 74, SDL_ALPHA_OPAQUE }; 
        mouseUpColor = { 164, 19, 60, SDL_ALPHA_OPAQUE }; 
    }
    else
    {
        mouseOutColor = { 255, 143, 163, SDL_ALPHA_OPAQUE }; 
        mouseOverMotionColor = { 95, 89, 191, SDL_ALPHA_OPAQUE };
        mouseDownColor = { 91, 191, 116, SDL_ALPHA_OPAQUE };
        mouseUpColor = { 95, 89, 191, SDL_ALPHA_OPAQUE };
    }
}

bool Sudoku::Cell::isEditable() const
{
    return editable;
}

// void Sudoku::Cell::handleKeyboardEvent(const SDL_Event* event, SDL_Texture* textureCache[])
// {
//     if (event->key.keysym.sym == SDLK_BACKSPACE && numberChar != ' ')
//     {
//         numberChar = ' ';
//         setTexture(textureCache[0]);
//     }
//     else if (event->type == SDL_TEXTINPUT)
//     {
//         if (strlen(event->text.text) == 1 && isdigit(event->text.text[0]) && event->text.text[0] != '0')
//         {
//             numberChar = event->text.text[0];  
//             int index = numberChar - '0'; 
//             setTexture(textureCache[index]);
//         }
//     }
// }

bool Sudoku::Cell::isCorrect() const
{
    return numberChar == solutionChar;
}
