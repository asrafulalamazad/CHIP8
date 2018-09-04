build:
	g++ -Wall -Wextra chip8.cc -o chip8.exe -lSDL2

clean:
	del chip8.exe
