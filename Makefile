all:
	g++ -I dep/include -L dep/lib -o main main.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image 