#include <cstdio>
#include <stdint.h>
#include <memory>
#include <cstdlib>
#include <cstring>
#include <SDL2/SDL.h>
#undef main

const int PIXEL_SIZE = 16; //PIXELS

class Chip8{
	public:
		// Chip8();
		bool ReadMemoryImage(const char *fname);
		void Reset();
		uint8_t mem[4096]{};
		uint8_t v[16]{};
		uint16_t i{};
		uint16_t pc{};
};

//dev.krzaq.cc
struct FileDeler{
	void operator()(FILE *ptr) const{
		fclose(ptr);
	}
};

bool Chip8::ReadMemoryImage(const char *fname){
 std::unique_ptr<FILE,FileDeler> f;
 f.reset(fopen(fname,"rd"));
	if(f == nullptr){
		return false;
	}
	return fread(mem + 512, 1 ,4096 - 512,f.get()) > 0;
}

void Chip8::Reset(){
	memset(mem,0,sizeof(mem));
	pc = 0x512;
	for (size_t i = 0; i < 16; i++) {
		v[i] = 0;
	}
	this->i = 0;
}

int main(int argc, char const *argv[]){
	if(argc != 2 ){
		puts("Usage : chip8 <fileName> ");
		return 1;
	}
	auto vm = std::make_unique<Chip8>();
	vm->Reset();
	if( vm->ReadMemoryImage(argv[1])){
		puts("Faild to read RAM images");
		return 2;
	}

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_EVENTS ) != 0){
		puts("SDL Falids");
		return 3;
	}
	SDL_Window *win = SDL_CreateWindow("CHIP8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 64 * PIXEL_SIZE, 32 * PIXEL_SIZE, SDL_WINDOW_SHOWN);
	if (win == nullptr){
		puts("SDL Falids");
		SDL_Quit();
		return 4;
	}
	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	if (ren == nullptr){
		SDL_DestroyWindow(win);
		puts("SDL Falids");
		SDL_Quit();
		return 5;
	}

	SDL_Surface *surface = SDL_GetWindowSurface(win);




	uint8_t *px = (uint8_t *)surface->pixels;
	px[ 256 * surface->pitch + 512 * 4] = 0xff;
	SDL_Event e;
	bool the_end =false;
	while(!the_end){
		while(SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				the_end = true;
				break;
			}
		}
		SDL_UpdateWindowSurface(win);
	}

	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}
