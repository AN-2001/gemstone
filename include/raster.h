#ifndef RASTER_H
#define RASTER_H
#include "linkedList.h"
#include "colour.h"
#include "gem.h"
typedef struct pixel{
	int x, y;
	float col;
} Pixel;

typedef LinkedList RasterList;

typedef void (*Fill)(Gem img, int x, int y, float Lambdas[3], void *UserData);

RasterList rasterListCreate();
void rasterize(RasterList list, int x1, int y1, int x2, int y2, float col);


void drawTriangle(Gem img, int x1, int y1,
						   int x2, int y2,
						   int x3, int y3, Fill Fill, void *UserData);

void drawLine(Gem img, int x1, int y1,
					   int x2, int y2, Cindex col);
#endif // RASTER_H
