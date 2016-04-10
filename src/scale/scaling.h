#ifndef SCALE_H
#define SCALE_H

#include "../rledecode.h"
typedef struct Pixel{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
} Pixel;

int scaleImage(Frame *frame, int scalefactor);

#endif