#include <stdio.h>
#include "colour.h"
#include "boilerplate.h"
#include "config.h"
#include <math.h>


int main(int argc, const char *argv[]){
	parseArgs(argc, argv);
	const char *path = (pathi != -1 ? argv[pathi] : "out");	
	const char *palettePath = (palettei != -1 ? argv[palettei] : "default.csv");	

	Palette p = paletteLoad(palettePath);
	if(!p){
		fprintf(stderr, "couldn't load palette\n");
		return 1;
	}
	paletteSet(p);

	Gem img = gemCreate(WIDTH, HEIGHT);
	if(!img){
		fprintf(stderr, "couldn't open image\n");
		return 1;
	}
	for(int i = 0; i < WIDTH; i++){
		for(int j = 0; j < HEIGHT; j++){
			fillPixel(img, i, j, i / (WIDTH /PALETTE_COUNT));
		}
	}
	outputImage(img, path, ONE_IMG);

	paletteFree(p);
	gemDestroy(img);
	return 0;
}
