#include <cstdio>
#include <stdint.h>
#include <memory>

class Chip8{
	public:
		// Chip8();
		bool ReadMemoryImage(const char *fname);
		uint8_t mem[4096]{};
		uint8_t v[16]{};
		uint16_t i{};
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

int main(int argc, char const *argv[]){
	if(argc != 2 ){
		puts("Usage : chip8 <fileName> ");
		return 1;
	}
	auto vm = std::make_unique<Chip8>();
	if(!vm->ReadMemoryImage(argv[1])){
		puts("Failed to read RAM imagess");
		return 2;
	}

	return 0;
}
