#include "raster.h"
#include <stdlib.h>
#include "config.h"
#include <math.h>

struct point{
	double x, y;
};

struct pixel pixelCreate(int x, int y, float col, float b){
	return (struct pixel){x, y, col, b};
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
	ret->b = ((struct pixel*)pixel)->b;
	return ret;
}

static int sign(double x){
	if(x == 0)
		return 0;
	return (x > 0) * 2 - 1;
}
static void doStraight(LinkedList list, struct point p, int x2, int y2, float col){
	struct point d = {x2 - p.x, y2 - p.y};
	if(d.x == 0 && d.y == 0){
		return;
	}

	else if(d.x == 0){
		int s = sign(d.y);
		while(p.y != y2){
			p.y += s;
			struct pixel pixel = pixelCreate(p.x, p.y, col, 1);
			listInsert(list, &pixel);
	//		if(p.x > WIDTH || p.x < 0 || p.y > HEIGHT || p.y < 0)
	//			return;
		}
	}else{
		int s = sign(d.x);
		while(p.x != x2){
			p.x += s;
			struct pixel pixel = pixelCreate(p.x, p.y, col, 1);
			listInsert(list, &pixel);
			//if(p.x > WIDTH || p.x < 0 || p.y > HEIGHT || p.y < 0)
			//	return;
		}
	}
} 
void rasterize(LinkedList list, int x1, int y1, int x2, int y2, float col){
	struct point p = {x1, y1},	
	             dir = {x2 - x1, y2 - y1}, d1, d2,
				 q = {x1, y1};
	struct pixel pixel = pixelCreate(p.x, p.y, col, 1);

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
		//if(q.x > WIDTH || q.x < 0 || q.y > HEIGHT || q.y < 0)
		//	return;
		

		pixel = pixelCreate(q.x, q.y, col , 1);
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
