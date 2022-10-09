#include <stdio.h>
#include "colour.h"
#include "boilerplate.h"
#include "config.h"
#include <math.h>
#include "stdlib.h"

int main(int argc, const char *argv[])
{
	parseArgs(argc, argv);
	int 
		Pathi = getArg(PATH),
		Palettei = getArg(PALETTE);

	const char 
		*Path = (Pathi != EMPTY ? argv[Pathi] : "out"),
	    *PalettePath = (Palettei != EMPTY ? argv[Palettei] : "default.csv");

	Palette 
		p = paletteLoad(PalettePath);

	if (!p) {
		fprintf(stderr, "couldn't load palette\n");
		return 1;
	}

	paletteSet(p);
	Gem 
		Gem = gemCreate(WIDTH, HEIGHT);

	if (!Gem) {
		fprintf(stderr, "couldn't open image\n");
		return 1;
	}

	for (int i = 0; i < WIDTH; i++) {
		for (int j = 0; j < HEIGHT; j++) {
			fillPixel(Gem,
					  i,
					  j,
					  (i / (float)WIDTH) * paletteGetCount());
		}
	}

	gemWrite(Gem, Path, ONE_IMG);

	paletteFree(p);
	gemDestroy(Gem);
	return 0;
}
