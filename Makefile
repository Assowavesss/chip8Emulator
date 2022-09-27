build:
	g++ -Wall \
	src/*.cpp src/CHIP-8/*.cpp src/CHIP-8/CPU/CPU.cpp src/GUI/*.cpp -std=c++17 \
	-I include/SDL2 \
	-L lib \
	-lmingw32 \
	-lSDL2main \
	-lSDL2 \
	-o bin/test.exe