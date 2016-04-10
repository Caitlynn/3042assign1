#include <stdlib.h>
#include <stdio.h>

#include "tweening.h"

Frame** tweenFrame(Frame *frame1, Frame *frame2, int tweenfactor){
	int pixels = frame1->width * frame1->height;
	Frame **newframes = malloc(sizeof(Frame *) * tweenfactor); // create a list of frame pointers
	for(int j = 0; j< tweenfactor; j++){
		Frame *tFrame = malloc(sizeof(Frame));
		tFrame->width = frame1->width;
		tFrame->height = frame1->height;
		tFrame->framedata = malloc(sizeof(char) * pixels * 3);
		newframes[j] = tFrame;
		for(size_t i=0; i<pixels; i++){
			tFrame->framedata[i] = frame1->framedata[i] - ((int)frame1->framedata[i] - (int)frame2->framedata[i]) / (tweenfactor+1) * (j+1);
			tFrame->framedata[i+pixels] = frame1->framedata[i+pixels] - ((int)frame1->framedata[i+pixels] - (int)frame2->framedata[i+pixels]) / (tweenfactor+1) * (j+1);
			tFrame->framedata[i+2*pixels] = frame1->framedata[i+2*pixels] - ((int)frame1->framedata[i+2*pixels] - (int)frame2->framedata[i+2*pixels]) / (tweenfactor+1) * (j+1);
		}

	}
	return newframes;
}