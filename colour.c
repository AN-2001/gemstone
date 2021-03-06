#include "colour.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define BUFFER_SIZE 512
#define MAX_P_COUNT 64	

#define IS_VALID(idx) ((idx) >= 0 && (idx) < currPalette->count)
struct palette_t{
	Colour *p;
	unsigned count;
};

static Palette currPalette = NULL;
static Colour currColour = PACK(255, 255, 255);

void setCurrentColour(Colour c){
	currColour = c;
}

// palette code
Palette paletteLoad(const char *csvPath){
	Palette p = malloc(sizeof(*p));
	if(!p)
		return NULL;
	p->p = malloc(sizeof(*p->p) * MAX_P_COUNT);
	p->count = 0;
	if(!p->p){
		free(p);
		return NULL;
	}

	FILE *csv = fopen(csvPath, "r");
	if(!csv){
		free(p);
		return NULL;
	}
	char c = 0, currCol[BUFFER_SIZE] = {0};
	while(!feof(csv)){
		int i = 0;	
		while( (c = fgetc(csv)) != ','){
			if(c == EOF)
				break;
			currCol[i++] = c;
		}
		currCol[i] = '\0';
		p->p[p->count++] = strtol(currCol, NULL, 16);
	}
	fclose(csv);
	printf("loaded %s with %d colours\n", csvPath, p->count);
	return p;

}
void paletteFree(Palette p){
	if(p)
		free(p->p);
	free(p);
}
void paletteSet(Palette p){
	if(!p)
		return;
	currPalette = p;
}
int paletteGetCount(){
	if(!currPalette)
		return 0;
	return currPalette->count;
}

// interpolation code //
static Colour linearInterp(long double t, int c1, int c2){
	if(!currPalette)
		return currColour;
	if(!IS_VALID(c1) || !IS_VALID(c2))
		return currColour;

	Colour color1 = currPalette->p[c1];
	Colour color2 = currPalette->p[c2];
	int r = R(color1) + t * (R(color2) - R(color1)),
	    g = G(color1) + t * (G(color2) - G(color1)),
	    b = B(color1) + t * (B(color2) - B(color1));
	return PACK(r, g, b);
}

static Colour paletteInterp(float v){
	v = fmax(fmin(v, currPalette->count - 1), 0);
	int lower = floorf(v);
	int higher = ceilf(v);
	float frac = v - lower;
	return linearInterp(frac, lower, higher);
}

/////////////////////////////////////


// drawing code
void setBackground(Gem img, Cindex index){
	if(!currPalette)
		return;
	Colour c = IS_VALID(index) ? paletteInterp(index) : currColour;
	gemSetBackGround(img, c);
} 

void fillPixel(Gem img, int x, int y, Cindex index){
	if(!currPalette)
		return;
	Colour c = IS_VALID(index) ? paletteInterp(index) : currColour;
	gemSetPixel(img, x, y, c);
}

//void fillRect(Gem *img, int x, int y, int w, int h, Cindex index){
//	if(!currPalette)
//		return;
//	Colour c = IS_VALID(index) ? currPalette->p[index] : currColour;
//}
//void fillCircle(Gem *img, int x, int y, int r, Cindex index){
//	if(!currPalette)
//		return;
//	Colour c = IS_VALID(index) ? currPalette->p[index] : currColour;
//}

