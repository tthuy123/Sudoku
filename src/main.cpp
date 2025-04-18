#include "Menu.h"

int main(int argc, char* argv[])
{
	// Create Sudoku game object
	Menu M;
    Menu menu;
    if (!menu.init()) return -1;
    menu.run();
    return 0;
    
	}