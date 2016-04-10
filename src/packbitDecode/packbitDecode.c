#include "packbitDecode.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>


int packbitDecode(FILE *file, Frame *frame){
	// get the first element of the file and check it's the end of the file or 'K'
	char seperator = fgetc(file);
	if (seperator == 'E' || (int)seperator == EOF) {
		return false; // there are no more frames to decode
	}
	if (seperator != 'K') {
		fprintf(stderr, "the frame data didn't start with 'K'\n");
		return false;
	}
	//calculate the data size
	size_t size = frame->height * frame->width * 3;
	frame->framedata = malloc(sizeof(unsigned char) * size);
	size_t i = 0;
	while (i < size) {
		int blockheader = fgetc(file);
		if (blockheader == EOF) {
			fprintf(stderr, "unexpected eof\n");
			return false;
		}
		
		//change the integer(2 bytes) to a char (one byte)
		blockheader = (char)blockheader;
		if (blockheader < 0) {
			// if the header is negative
			unsigned char symbol = fgetc(file);
			for (int n = 0; n < 2 - blockheader; n++) {
				frame->framedata[i] = symbol;
				i++;
			}
		} else {
			// if the header is positive
			for (int n = 0; n < blockheader + 1; n++) {
				frame->framedata[i] = fgetc(file);
				i++;
			}
		}
	}
	return true;
}

