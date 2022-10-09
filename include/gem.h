#ifndef GEM
#define GEM
#define ONE_IMG 1,1
typedef unsigned Colour;
typedef struct gemImg_t *Gem;
typedef unsigned *Pixels;

Gem gemCreate(unsigned width, unsigned height);

int gemWrite(Gem img, const char *path, int i, int total);
Gem gemRead(const char *path);
unsigned gemWidth(Gem img);
unsigned gemHeight(Gem img);
void gemSetBackGround(Gem img, Colour col);
void gemSetPixel(Gem img,int x, int y, Colour col);
Colour gemGetPixel(Gem img, int x, int y);
Pixels gemGetPtr(Gem img);

// just a progress bar kek
void printProgress(int curr, int total);
//

void gemDestroy(Gem img);
#endif // GEM
