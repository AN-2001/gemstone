#ifndef COLOUR_H
#define COLOUR_H
#include "gemWriter.h"
#define USE_CUSTOM_COLOUR -1
#define PALETTE_COUNT (paletteGetCount())
#define PALETTE_MAX_INDEX (paletteGetCount() - 1)
#define R(c) (((c >> 16) & 0xff))
#define G(c) (((c >> 8) & 0xff))
#define B(c) (((c) & 0xff))
#define PACK(r, g, b) (((b) & 0xff) | ((g & 0xff) << 8) | ((r & 0xff) << 16)) 
typedef int Colour;
typedef float Cindex;
typedef struct palette_t *Palette;

// set the current background colour
// TODO: add lazy evaluation?
// index is a float between 0 and PALETTE_MAX_INDEX
void setBackground(Gem img, Cindex index);

// fill the pixel at (x,y)
// index is a float between 0 and PALETTE_MAX_INDEX
void fillPixel(Gem img, int x, int y, Cindex index);

// set the current custom colour
void setCurrentColour(Colour c);

// loads a palette from a CSV file
Palette paletteLoad(const char *csvPath);

// frees a palette
void paletteFree(Palette p);

// sets the current palette
void paletteSet(Palette p);

// gets the number of colours in the current palette
int paletteGetCount();

#endif // COLOUR_H
