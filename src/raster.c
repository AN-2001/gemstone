#include "raster.h"
#include "gem.h"
#include <stdlib.h>
#include <math.h>
#define EPSILON 1e-5

struct point{
	double x, y;
};



static struct pixel pixelCreate(int x, int y, float col);
static void *pixelCopy(void *pixel);

RasterList rasterListCreate(){
	return listCreate(pixelCopy, free);
}
struct pixel pixelCreate(int x, int y, float col){
	return (struct pixel){x, y, col};
}

void *pixelCopy(void *pixel){
	if(!pixel)
		return NULL;
	struct pixel *ret = malloc(sizeof(*ret));
	if(!ret)
		return NULL;
	ret->x = ((struct pixel*)pixel)->x;
	ret->y = ((struct pixel*)pixel)->y;
	ret->col = ((struct pixel*)pixel)->col;
	return ret; }

static int sign(double x){
	if(x == 0)
		return 0;
	return (x > 0) * 2 - 1;
}
static void doStraight(RasterList list, struct point p, int x2, int y2, float col){
	struct point d = {x2 - p.x, y2 - p.y};
	if(d.x == 0 && d.y == 0){
		return;
	}

	else if(d.x == 0){
		int s = sign(d.y);
		while(p.y != y2){
			p.y += s;
			struct pixel pixel = pixelCreate(p.x, p.y, col);
			listInsert(list, &pixel);
		}
	}else{
		int s = sign(d.x);
		while(p.x != x2){
			p.x += s;
			struct pixel pixel = pixelCreate(p.x, p.y, col);
			listInsert(list, &pixel);
		}
	}
} 
void rasterize(RasterList list, int x1, int y1, int x2, int y2, float col){
	struct point p = {x1, y1},	
	             dir = {x2 - x1, y2 - y1}, d1, d2,
				 q = {x1, y1};
	struct pixel pixel = pixelCreate(p.x, p.y, col);

	listInsert(list, &pixel);

	if(dir.x == 0 || dir.y == 0){
		 doStraight(list, p, x2, y2, col);
		 return;
	}

	d1 = (struct point){sign(dir.x), (dir.y / dir.x) * sign(dir.x)};
	d2 = (struct point){(dir.x / dir.y) * sign(dir.y), sign(dir.y)};
	if(fabs(d1.y) <= fabs(d2.x))
		dir = d1;
	else
		dir = d2;

	while(q.x != x2 || q.y != y2){
		// apply the last direction	
		p = (struct point){p.x + dir.x, p.y + dir.y};
		q = (struct point){sign(dir.x) > 0 ? ceil(fabs(p.x)) * sign(p.x) : floor(fabs(p.x)) * sign(p.x),
		 				   sign(dir.y) > 0 ? ceil(fabs(p.y)) * sign(p.y) : floor(fabs(p.y)) * sign(p.y)};
		//p = q;
		
		pixel = pixelCreate(q.x, q.y, col);
		listInsert(list, &pixel);

		// calculate the new direction
		dir = (struct point){x2 - p.x, y2 - p.y};

		// check whether we can just go straight
		if(dir.x == 0 || dir.y == 0){
			doStraight(list, q, x2, y2, col);
			continue;
		}

		d1 = (struct point){sign(dir.x), (dir.y / dir.x) * sign(dir.x)};
		d2 = (struct point){(dir.x / dir.y) * sign(dir.y), sign(dir.y) };

		double rx = fabs(q.x - p.x),
		       ry = fabs(q.y - p.y);
		if(rx != 0 || ry != 0){
			if(rx != 0)
				d1 = (struct point){d1.x * rx, d1.y * rx};
			else
				d2 = (struct point){d2.x * ry, d2.y * ry};
		}

		if(fabs(d1.y) <= fabs(d2.x))
			dir = d1;
		else
			dir = d2;
	}
}

static void draw(void *pixel, void *img){
	struct pixel p = *(struct pixel*)pixel;
	Gem g = *(Gem*)img;
	fillPixel(g, p.x, p.y, p.col);
}

static void updateRows(void *pixel, void *rowsArr){
	struct pixel p = *(struct pixel*)pixel;
	int (*arr)[2] = (int(*)[2])rowsArr;
	unsigned height = arr[0][1];
	arr++;

	if(p.y < 0 || p.y >= height)
		return;
	arr[p.y][0] = fmin(arr[p.y][0], p.x);
	arr[p.y][1] = fmax(arr[p.y][1], p.x);
}
void drawTriangle(Gem img, int x1, int y1,
						   int x2, int y2,
						   int x3, int y3, Fill Fill, void *UserData){
	if(!img) 
		return;

	RasterList e1 = rasterListCreate();
	RasterList e2 = rasterListCreate();
	RasterList e3 = rasterListCreate();

	rasterize(e1, x1, y1, x3, y3, USE_CUSTOM_COLOUR);
	rasterize(e2, x1, y1, x2, y2, USE_CUSTOM_COLOUR);
	rasterize(e3, x2, y2, x3, y3, USE_CUSTOM_COLOUR);
	unsigned height = gemHeight(img);
	unsigned width = gemWidth(img);
	int (*xvals)[2] = malloc(sizeof(*xvals) * (height + 1));
	xvals[0][1] = height; 
	xvals[0][0] = 0; 

	for(int i = 1; i <= height; i++){
		xvals[i][0] = width;
		xvals[i][1] = 0;
	}

	listIter(e1, updateRows, xvals);
	listIter(e2, updateRows, xvals);
	listIter(e3, updateRows, xvals);
	float 
		Det = (y2 - y3) * (x1 - x3) + (x3 - x2) * (y1 - y3);

	/* Only fill when the determinant is greater than 0. */
	if (fabs(Det) > EPSILON) {
		for(int i = 0; i < height; i++){
			for(int j = xvals[i + 1][0]; j <= xvals[i + 1][1]; j++){
				/* Find barycentric coordinates. */
				float Lambda[3];	
				float 
					x = j,
					y = i,
					p1 = (y2 - y3) * (x - x3) + (x3 - x2) * (y - y3),
					p2 = (y3 - y1) * (x - x3) + (x1 - x3) * (y - y3);

				/* Try to minimize floating point error with fmin and fmax. */
				Lambda[0] = fmin(fmax(p1 / Det, 0.f), 1.f);
				Lambda[1] = fmin(fmax(p2 / Det, 0.f), 1.f);
				Lambda[2] = fmin(fmax(1.f - Lambda[0] - Lambda[1], 0.f), 1.f);

				Fill(img, j, i, Lambda, UserData);
			}
		}
	}
	
	//setCurrentColour(PACK(255, 255, 255));
	//listIter(e1, draw, &img);
	//listIter(e2, draw, &img);
	//listIter(e3, draw, &img);

	listFree(e1);
	listFree(e2);
	listFree(e3);
	free(xvals);
}

void drawLine(Gem img, int x1, int y1,
					   int x2, int y2, Cindex col){
	if(!img)
		return;
	RasterList list = rasterListCreate();
	rasterize(list, x1, y1, x2, y2, col);
	listIter(list, draw, &img);
	listFree(list);
}
