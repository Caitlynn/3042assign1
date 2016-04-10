#ifndef HEADERDECODE_H
#define HEADERDECODE_H
#include "../rledecode.h"
#include <stdio.h>

typedef struct Header{
	int width;
	int height;
} Header;

int headerDecode(FILE *rleFile, Header *header);
#endif