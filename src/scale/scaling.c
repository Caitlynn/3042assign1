#include "scaling.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

/*this function returns a Pixel of the input coordinate on a frame*/
Pixel getPixel(Frame *frame, int row, int col){
	size_t frameoffset = frame->width * frame->height;
	size_t gridoffset = col + row * frame->width;
	Pixel pixel = {
		.red = frame->framedata[gridoffset],
		.green = frame->framedata[gridoffset + frameoffset],
		.blue = frame->framedata[gridoffset + 2 * frameoffset]
	};
	return pixel;
}

/*this function takes a bitmap(frame), and a coordinate of a pixel, and place the pixel in the bitmap*/
void setPixel(Frame *bitmap, Pixel pixel, int row, int col){
	size_t bitmapoffset = bitmap->width * bitmap->height;
	size_t gridoffset = col + row * bitmap->width;
	bitmap->framedata[gridoffset] = pixel.red;
	bitmap->framedata[gridoffset + bitmapoffset] = pixel.green;
	bitmap->framedata[gridoffset + 2 * bitmapoffset] = pixel.blue;
}

/*this function is supposed to scale a frame by scalefactor*/
int scaleImage(Frame *frame, int scalefactor){
	Frame *bitmap = malloc(sizeof(Frame)); // scaleFrame is a bitmap-like frame with the scaled size
	bitmap->width = scalefactor * (frame->width-1) + 1;
	bitmap->height = scalefactor * (frame->height-1) + 1;
	unsigned int scaleddataSize = bitmap->width * bitmap->height * 3;
	bitmap->framedata = malloc(sizeof(char) * scaleddataSize);

	// place the original frame data into the bitmap 
	unsigned int row = frame->height;
	unsigned int col = frame->width;
	for(int j = 0; j < row; j++){
		for(int i = 0; i < col; i++){
			Pixel pixel = getPixel(frame, j, i);
			setPixel(bitmap, pixel, j*scalefactor, i*scalefactor); //because the new coordinates will be the old one times the scalefactor
		}	
	}
	
	//interpolate the rows
	for(int t = 0; t < row; t++){
		for(int j = 0; j < scalefactor - 1; j++){
			Pixel firstp = getPixel(frame,t, j);
			Pixel secondp = getPixel(frame,t, j + 1);
			for(int i = 0; i < scalefactor - 1; i++){
				Pixel newp = {
					.red = firstp.red + (((int)secondp.red - (int)firstp.red)/scalefactor) * (i + 1),
					.green = firstp.green + (((int)secondp.green - (int)firstp.green)/scalefactor) * (i + 1),
					.blue = firstp.blue + (((int)secondp.blue - (int)firstp.blue)/scalefactor) * (i + 1)
				};
				setPixel(bitmap, newp, t*scalefactor, j*scalefactor + i + 1);
			}	
		}
	}

	//interpolate the columns
	for(int t = 0; t < col; t++){
		for(int j = 0; j < scalefactor - 1; j++){
			Pixel firstp = getPixel(frame,j, t);
			Pixel secondp = getPixel(frame,j + 1, t);
			for(int i = 0; i < scalefactor - 1; i++){
				Pixel newp = {
					.red = firstp.red + (((int)secondp.red - (int)firstp.red)/scalefactor) * (i + 1),
					.green = firstp.green + (((int)secondp.green - (int)firstp.green)/scalefactor) * (i + 1),
					.blue = firstp.blue + (((int)secondp.blue - (int)firstp.blue)/scalefactor) * (i + 1)
				};
				setPixel(bitmap, newp, j*scalefactor + i + 1, t*scalefactor);
			}	
		}
	}



	free(frame->framedata);
	frame->width = bitmap->width;
	frame->height = bitmap->height; 
	frame->framedata = bitmap->framedata;
	free(bitmap);
	return true;
}
