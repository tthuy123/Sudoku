#include "Menu.h"

int main(int argc, char* argv[])
{
	Menu M;
    Menu menu;
    if (!menu.init()) return -1;
    menu.run();
    return 0;
    
	}