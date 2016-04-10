#ifndef DECODE_H
#define DECODE_H

#include "arguments/arguments.h"

typedef struct Frame{
	int height;
	int width;
	unsigned char *framedata;
} Frame;

typedef struct Header{
	int width;
	int height;
} Header;


#endif 