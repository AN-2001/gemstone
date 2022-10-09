#ifndef COLOUR_H
#define COLOUR_H
#include "gem.h"
#define USE_CUSTOM_COLOUR -1
#define R(c) (((c >> 16) & 0xff))
#define G(c) (((c >> 8) & 0xff))
#define B(c) (((c) & 0xff))
#define PACK(r, g, b) ((((unsigned)(b)) & 0xff)        |\
					  ((((unsigned)(g)) & 0xff) << 8)  |\
					  ((((unsigned)(r)) & 0xff) << 16) |\
					  (0xff << 24)) 

typedef unsigned Colour;
typedef float Cindex;
typedef struct palette_t *Palette;

extern Colour CurrentColour;

// set the current background colour
// TODO: add lazy evaluation?
// index is a float between 0 and PALETTE_MAX_INDEX
void setBackground(Gem img, Cindex index);

// fill the pixel at (x,y)
// index is a float between 0 and PALETTE_MAX_INDEX
void fillPixel(Gem img, int x, int y, Cindex index);

void fillPixelB(Gem img, int x, int y, Cindex index, float brightness);

// loads a palette from a CSV file
Palette paletteLoad(const char *csvPath);

// frees a palette
void paletteFree(Palette p);

// sets the current palette
void paletteSet(Palette p);

// gets the number of colours in the current palette
int paletteGetCount();

#endif // COLOUR_H
