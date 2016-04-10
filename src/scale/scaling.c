#include "scaling.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

/*this function is supposed to scale a frame by scalefactor, but I didnt get it work 
so I deleted it in case it has some errors to break my program*/
int scaleImage(Frame *frame, int scalefactor){
	Frame *scaledFrame = malloc(sizeof(Frame)); // scaleFrame is a bitmap-like frame with the scaled size
	scaledFrame->width = scalefactor * (frame->width-1) + 1;
	scaledFrame->height = scalefactor * (frame->height-1) + 1;
	unsigned int scaleddataSize = scaledFrame->width * scaledFrame->height * 3;
	scaledFrame->framedata = malloc(sizeof(char) * scaleddataSize);

	
	
	free(frame->framedata);
	frame->width = scaledFrame->width;
	frame->height = scaledFrame->height;
	frame->framedata = scaledFrame->framedata;
	free(scaledFrame);
	return true;
}
