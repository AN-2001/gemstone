#include <iostream>
#include "boilerplate.h"
namespace Gemstone{
	extern "C"{
		#include "gem.h"
		#include "colour.h"
		#include "config.h"
	}
}


int main(int argc, const char *argv[])
{
	Gemstone::parseArgs(argc, argv);
	int 
		Pathi = Gemstone::getArg(PATH),
		Palettei = Gemstone::getArg(PALETTE);

	const char 
		*Path = (Pathi != EMPTY ? argv[Pathi] : "out"),
	    *PalettePath = (Palettei != EMPTY ? argv[Palettei] : "default.csv");	

	Gemstone::Palette 
		p = Gemstone::paletteLoad(PalettePath);

	if (!p) {
		std::cout << "couldn't load palette" << std::endl;
		return 1;
	}

	paletteSet(p);
	Gemstone::Gem 
		Gem = Gemstone::gemCreate(WIDTH, HEIGHT);

	if (!Gem) {
		std::cout << "couldn't open image" << std::endl;
		return 1;
	}

	for (int i = 0; i < WIDTH; i++) {
		for (int j = 0; j < HEIGHT; j++) {
			Gemstone::fillPixel(Gem, 
							i, 
							j, 
							(i / ((float)WIDTH /Gemstone::paletteGetCount())));
		}
	}

	Gemstone::gemWrite(Gem, Path, ONE_IMG);
	Gemstone::paletteFree(p);
	Gemstone::gemDestroy(Gem);
	return 0;
}
