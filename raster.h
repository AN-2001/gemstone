#ifndef RASTER_H
#define RASTER_H
#include "linkedList.h"
struct pixel{
	int x, y;
	float col;
	float b;
};

struct pixel pixelCreate(int x, int y, float col, float b);

void *pixelCopy(void *pixel);

void rasterize(LinkedList list, int x1, int y1, int x2, int y2, float col);


#endif // RASTER_H
