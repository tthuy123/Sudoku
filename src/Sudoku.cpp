#include "Sudoku.h"

Sudoku::Sudoku::Sudoku()
	: windowHeight(600), windowWidth(1000),
	  gridHeight(450), gridWidth(450),
	  gridRows(9), gridCols(9),
	  window(nullptr), renderer(nullptr),
	  totalTextures(TOTAL_TEXTURE_CACHE), textureCache{ nullptr },
	  font(nullptr), fontSize(gridHeight / 13),
	  selectedNumber(0), selectedCell(-1),
	  newGameRequested(true), checkSolutionRequested(false),
	  hintRequested(false), completed(false),
	  clearColour({ 255, 255, 255, SDL_ALPHA_OPAQUE })
{
}

Sudoku::Sudoku::~Sudoku() {
	freeTextures();
	close();
}

bool Sudoku::Sudoku::initialiseSDL() {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cout << "SDL init error: " << SDL_GetError() << std::endl;
		return false;
	}

	if (TTF_Init() == -1) {
		std::cout << "TTF init error: " << TTF_GetError() << std::endl;
		return false;
	}

	window = SDL_CreateWindow("Sudoku", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if (!window || !renderer) {
		std::cout << "Window/Renderer creation error: " << SDL_GetError() << std::endl;
		return false;
	}

	font = TTF_OpenFont("assets/Lexend-SemiBold.ttf", fontSize);
	if (!font) {
		std::cout << "Font load error: " << TTF_GetError() << std::endl;
		return false;
	}

	return true;
}

inline int Sudoku::Sudoku::getIndex(int row, int col) const {
	return row * gridCols + col;
}

void Sudoku::Sudoku::loadTexture(SDL_Texture*& texture, const char* text, SDL_Color& fontColour) {
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, fontColour);
	if (!textSurface) {
		std::cout << "Text surface error: " << TTF_GetError() << std::endl;
		return;
	}

	texture = SDL_CreateTextureFromSurface(renderer, textSurface);
	if (!texture) {
		std::cout << "Could not create texture from surface! Error: " << SDL_GetError() << std::endl;
	}

	SDL_FreeSurface(textSurface);
}

void Sudoku::Sudoku::preloadTextures() {
	SDL_Color fontColour = { 0, 0, 0, SDL_ALPHA_OPAQUE };
	loadTexture(textureCache[0], " ", fontColour);

	for (int num = 1; num < 10; num++) {
		char temp[2] = { static_cast<char>('0' + num), '\0' };
		loadTexture(textureCache[num], temp, fontColour);
	}

	loadTexture(textureCache[10], "check", fontColour);
	loadTexture(textureCache[11], "hint", fontColour);
	loadTexture(textureCache[12], "solution", fontColour);
	loadTexture(textureCache[13], "new", fontColour);
}

void Sudoku::Sudoku::createInterfaceLayout() {
	const int padding = 25;
	int gridStartX = padding * 3;
	int gridStartY = padding * 3;
	int cellSize = gridWidth / 9;

	const SDL_Color BLACK = { 0, 0, 0, SDL_ALPHA_OPAQUE };
	const int THIN_BORDER = 1;
	const int THICK_BORDER = 5;

	for (int row = 0; row < 9; row++) {
		for (int col = 0; col < 9; col++) {
			int x = gridStartX + col * cellSize + (col / 3) * THICK_BORDER + col * THIN_BORDER;
			int y = gridStartY + row * cellSize + (row / 3) * THICK_BORDER + row * THIN_BORDER;
			SDL_Rect rect = { x, y, cellSize, cellSize };
			grid[getIndex(row, col)].setButtonRect(rect);
		}
	}

	int rightPanelX = gridStartX + gridWidth + padding * 6;
	int rightPanelY = gridStartY + padding;
	int buttonWidth = 150;
	int buttonHeight = 50;

	Button* sideButtons[] = { &checkButton, &hintButton, &solutionButton, &newButton };
	for (int i = 0; i < 4; ++i) {
		sideButtons[i]->setTexture(textureCache[10 + i]);
		SDL_Rect rect = { rightPanelX, rightPanelY + i * (buttonHeight + padding), buttonWidth, buttonHeight };
		sideButtons[i]->setButtonRect(rect);
	}

	int bottomY = rightPanelY + 4 * (buttonHeight + padding) + padding * 2;
	for (int i = 0; i < 9; ++i) {
		numberButtons[i].setTexture(textureCache[i + 1]);
		SDL_Rect rect = {
			rightPanelX + i * (cellSize * 0.9) - padding*4,
			bottomY,
			cellSize*0.8,
			cellSize*0.8
		};
		numberButtons[i].setButtonRect(rect);
	}
}

void Sudoku::Sudoku::generateSudoku() {
	int generatedGrid[81] = {};
	int solvedGrid[81] = {};

	Generator generator;
	generator.generate(generatedGrid, solvedGrid);

	for (int i = 0; i < TOTAL_CELL; i++) {
		grid[i].setNumber(generatedGrid[i]);
		grid[i].setSolution(solvedGrid[i]);
		solution[i] = solvedGrid[i];

		grid[i].setEditable(generatedGrid[i] == 0);
		grid[i].setTexture(textureCache[generatedGrid[i]]);
		grid[i].centerTextureRect();
	}
}

void Sudoku::Sudoku::freeTextures() {
	for (int i = 0; i < totalTextures; i++) {
		if (textureCache[i]) {
			SDL_DestroyTexture(textureCache[i]);
			textureCache[i] = nullptr;
		}
	}
}

void Sudoku::Sudoku::handleEvents(SDL_Event& event, bool& quit) {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            quit = true;
            return;
        }

        if (event.type == SDL_MOUSEMOTION ||
            event.type == SDL_MOUSEBUTTONDOWN ||
            event.type == SDL_MOUSEBUTTONUP) {

            newButton.getMouseEvent(&event);
            checkButton.getMouseEvent(&event);
            hintButton.getMouseEvent(&event);
			solutionButton.getMouseEvent(&event);

            for (int i = 0; i < 9; ++i)
                numberButtons[i].getMouseEvent(&event);

            for (int i = 0; i < TOTAL_CELL; ++i) {
				if (grid[i].isEditable()) {
					grid[i].getMouseEvent(&event);
				}
			}
        }

        if (event.type == SDL_MOUSEBUTTONUP) {
            if (newButton.getMouseEvent(&event) == ButtonState::MouseUp)
                newGameRequested = true;

            if (checkButton.getMouseEvent(&event) == ButtonState::MouseUp)
                checkSolutionRequested = true;

            if (hintButton.getMouseEvent(&event) == ButtonState::MouseUp && selectedCell != -1)
                hintRequested = true;
			if (solutionButton.getMouseEvent(&event) == ButtonState::MouseUp) {
				for (int i = 0; i < TOTAL_CELL; ++i) {
					grid[i].setNumber(solution[i]);
					grid[i].setTexture(textureCache[solution[i]]);
					grid[i].centerTextureRect();
					grid[i].setEditable(false);
				}
				completed = true;
			}
			bool clickedNumberButton = false;
			bool clickedCell = false;
			
			for (int i = 0; i < 9; ++i) {
				if (numberButtons[i].getMouseEvent(&event) == ButtonState::MouseUp) {
					selectedNumber = i + 1;
					clickedNumberButton = true;
					for (int j = 0; j < 9; ++j)
						numberButtons[j].setSelected(j == i);
				}
			}
			
			for (int i = 0; i < TOTAL_CELL; ++i) {
				if (grid[i].isEditable() &&
					grid[i].getMouseEvent(&event) == ButtonState::MouseUp) {
					
					selectedCell = i;
					clickedCell = true;
					for (int j = 0; j < TOTAL_CELL; ++j)
						grid[j].setSelected(j == i);
				}
			}
			
			if (event.type == SDL_MOUSEBUTTONUP &&
				event.button.button == SDL_BUTTON_LEFT &&
				!clickedNumberButton && !clickedCell) {
			
				selectedNumber = 0;
				for (int j = 0; j < 9; ++j)
					numberButtons[j].setSelected(false);
			}
    
}
    
}
}

void Sudoku::Sudoku::updateGameState() {
	if (newGameRequested) {
		generateSudoku();
		selectedNumber = 0;
		selectedCell = -1;
		completed = false;
		for (int i = 0; i < TOTAL_CELL; ++i) {
			grid[i].setSelected(false);
		}
		for (int i = 0; i < 9; ++i) {
			numberButtons[i].setSelected(false);
		}
		newGameRequested = false;
	}

	if (checkSolutionRequested) {

		completed = true;
		for (int i = 0; i < TOTAL_CELL; ++i) {
			if (!grid[i].isCorrect()) {
				completed = false;
				break;
			}
		}
		messageText = completed ? "You did it!" : "Try again!";
		messageStartTime = time(nullptr);
		showMessage = true;
	
		checkSolutionRequested = false;
	}

	if (hintRequested && selectedCell != -1) {
		grid[selectedCell].setNumber(solution[selectedCell]);
		grid[selectedCell].setTexture(textureCache[solution[selectedCell]]);
		grid[selectedCell].centerTextureRect();
		grid[selectedCell].setEditable(false);
		hintRequested = false;
	}

	if (selectedCell != -1 && selectedNumber != 0 && grid[selectedCell].isEditable()) {
		grid[selectedCell].setNumber(selectedNumber);
		grid[selectedCell].setTexture(textureCache[selectedNumber]);
		grid[selectedCell].centerTextureRect();
	}
}

void Sudoku::Sudoku::renderUI() {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);

	for (int i = 0; i < TOTAL_CELL; ++i) {
		grid[i].renderButton(renderer);
		grid[i].renderTexture(renderer);
	}

	checkButton.renderButton(renderer);
	checkButton.renderTexture(renderer);
	checkButton.centerTextureRect();

	hintButton.renderButton(renderer);
	hintButton.renderTexture(renderer);
	hintButton.centerTextureRect();

	solutionButton.renderButton(renderer);
	solutionButton.renderTexture(renderer);
	solutionButton.centerTextureRect();

	newButton.renderButton(renderer);
	newButton.renderTexture(renderer);
	newButton.centerTextureRect();

	for (int i = 0; i < 9; ++i) {
		numberButtons[i].renderButton(renderer);
		numberButtons[i].renderTexture(renderer);
		numberButtons[i].centerTextureRect();
	}
	if (showMessage) {
		time_t currentTime = time(nullptr);
		double elapsedTime = difftime(currentTime, messageStartTime); 
	
		if (elapsedTime < 2) {
			SDL_Color color = { 0, 0, 0, 255 };
			SDL_Surface* surface = TTF_RenderText_Solid(font, messageText.c_str(), color);
			SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
			SDL_Rect rect = { 800, 550, surface->w, surface->h };
			SDL_RenderCopy(renderer, texture, nullptr, &rect);
			SDL_FreeSurface(surface);
			SDL_DestroyTexture(texture);
		} else {
			showMessage = false; 
		}
	}

	SDL_RenderPresent(renderer);
}

void Sudoku::Sudoku::play() {

	if (!initialiseSDL())
	{
		close();
	}
	preloadTextures();
	createInterfaceLayout();
	generateSudoku();
	bool quit = false;
	SDL_Event event;

	while (!quit) {
		handleEvents(event, quit);
		updateGameState();
		renderUI();
	}
}

void Sudoku::Sudoku::close() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_CloseFont(font);

	renderer = nullptr;
	window = nullptr;
	font = nullptr;

	SDL_Quit();
	TTF_Quit();
}
