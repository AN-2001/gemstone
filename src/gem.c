#include "gem.h"
#include <stdlib.h>
#include <stdio.h>
#include "colour.h"
#include <memory.h>
#include <ctype.h>
//#include "boilerplate.h"
#define GET_INDEX(x, y) ((x) + (y)*img->width)
#define MAGIC "P6\n"
#define MAGIC_HEX 0xa3650
#define OUT_SIZE 64 
#define PRINT_PERIOD 20
enum{
	RED = 0,
	GREEN,
	BLUE
};
struct gemImg_t{
	unsigned width;
	unsigned height;
	/*char (*pixels)[3];*/
	Pixels pixels;
};

static void gemWrite_(Gem img, const char *path);


Gem gemCreate(unsigned width, unsigned height){
	Gem ret = malloc(sizeof(*ret) * width * height);
	if(!ret)
		return NULL;
	ret->width = width;
	ret->height = height;
	ret->pixels = malloc(sizeof(*ret->pixels) * ret->width * ret->height);
	if(!ret->pixels){
		free(ret);
		return NULL;
	}
	memset(ret->pixels, 0, width * height * sizeof(*ret->pixels));
	return ret;
}

int gemWrite(Gem img, const char *path, int i, int total){
	if(!img || !path)
		return 0;
	char out[OUT_SIZE] = {0};
	if(total == 1){
		printProgress(total, total);
		sprintf(out, "%s.ppm", path);
		gemWrite_(img, out);
		return total;
	}

	if(i % PRINT_PERIOD == 0)
		printProgress(i, total);
	sprintf(out, "%s%d.ppm", path, i);
	gemWrite_(img, out);
	return i;
}

Gem gemRead(const char *path){
	if(!path)
		return NULL;
	Gem gem = NULL;
	FILE *file = fopen(path, "r");
	if(!file)
		return gem;
	char magic[3];
	fread(magic, 3, 1, file);
	if(*(int*)magic != MAGIC_HEX){
		fclose(file);
		return gem;
	}
	long offset = ftell(file);
	fseek(file, 0, SEEK_END);
	long size = ftell(file);
	fseek(file, offset, SEEK_SET);
	char *buff = malloc(size * sizeof(*buff));
	char *start = buff;
	if(!buff){
		fclose(file);
		return gem;
	}
	fread(buff, size, 1, file);
	fclose(file);



	// read width and height
	int width = atoi(buff);
	for(;!isspace(*buff); buff++);
	buff++;
	int height = atoi(buff);
	for(;!isspace(*buff); buff++);
	buff++;
	for(;!isspace(*buff); buff++);
	buff++;

	gem = gemCreate(width, height);
	if(!gem){
		free(buff);
		fclose(file);
		return gem;
	}
	// read the pixel info
	memmove(gem->pixels, buff, width * height * sizeof(*gem->pixels));
	free(start);
	return gem;
}


static void gemWrite_(Gem img, const char *path){
	if(!img || !path)
		return;
	FILE *file = fopen(path, "w");
	if(!file)
		return;

	fprintf(file, MAGIC"%u %u 255\n", img->width, img->height);
	// fwrite(img->pixels, sizeof(*img->pixels), img -> width * img -> height, file);
	
	for (int i = 0; i < img -> width * img -> height; i++) {
		fwrite(img->pixels + i, 1, 3, file);
	}

	fclose(file);
}

void gemDestroy(Gem img){
	if(!img)
		return;
	free(img->pixels);
	free(img);
}

void gemSetBackGround(Gem img, Colour col){
	if(!img)
		return;
	for(int i = 0; i < img->width * img->height; i++){
		//img->pixels[i][RED] = R(col);
		//img->pixels[i][GREEN] = G(col);
		//img->pixels[i][BLUE] = B(col);
		img->pixels[i] = col;
	}
}
void gemSetPixel(Gem img,int x, int y, Colour col){
	if(!img)
		return;
	if(x < 0 || x>= img->width || y < 0 || y >= img->height)
		return;
	int index = GET_INDEX(x, y);
	// img->pixels[index][RED] = R(col);
	// img->pixels[index][GREEN] = G(col);
	// img->pixels[index][BLUE] = B(col);
	img->pixels[index] = col;
}
Colour gemGetPixel(Gem img, int x, int y){
	if(!img)
		return 0;
	if(x < 0 || x>= img->width || y < 0 || y >= img->height)
		return 0;
	int index = GET_INDEX(x, y);
	// int r = img->pixels[index][RED];
	// int g = img->pixels[index][GREEN];
	// int b = img->pixels[index][BLUE];
	return img->pixels[index];
}

Pixels gemGetPtr(Gem img){
	return img->pixels;
}

unsigned gemWidth(Gem img){
	if(!img)
		return 0;
	return img->width;
}

unsigned gemHeight(Gem img){
	if(!img)
		return 0;
	return img->height;
}

// progress bar stuff //
#define NUM_PROGRESS_BARS 40
#define FULL '='
#define ARROW '>'
#define EMPTY '-'
#define GO_TO_LINE_START "[0`"

void printProgress(int curr, int total){
	double precentage = (double)curr/total;
	int numFull = precentage * NUM_PROGRESS_BARS;
	printf("%s", GO_TO_LINE_START);
	for(int i = 0; i < NUM_PROGRESS_BARS; i++){
		putc(i <= numFull ? (i == numFull ? ARROW : FULL)  : EMPTY, stdout);
	}
	printf("|%3d%%", (int)(precentage * 100));
	fflush(stdout);
}
///
