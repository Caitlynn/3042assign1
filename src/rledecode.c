#include "rledecode.h"
#include <stdbool.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/* this program decodes an rleplay file and prodcus a sequence of images in PPM format
*/ 


/*
this function returns 1 if width and height are pos int,
otherwise returns 0 and store width and height.
*/
int headerDecode(FILE *rleFile, Header *header){
	//check if it's the end of the file
	if (feof(rleFile)){
		return false;
	} else {
		//get the width and the height
		if (fscanf(rleFile, "RLEPLAYv1\n%d %d\n",&(header->width), &(header->height)) != 2) {
			fprintf(stderr, "couldn't decode the header\n");
			return false;
		}
	}

	if (header->width < 1 || header->height < 1){
		return false;
	}
	return true;
}

int packbitDecode(FILE *file, Frame *frame){
	char seperator = fgetc(file);
	if (seperator != 'K') {
		fprintf(stderr, "the frame data didn't start with 'K'\n");
		return false;
	}
	size_t size = frame->height * frame->width * 3;
	frame->framedata = malloc(sizeof(unsigned char) * size);
	size_t i = 0;
	while (i < size) {
		int blockheader = fgetc(file);
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


// scale an image using blinear interpolation
void scaleImage(char *scaleRange,char *images){

}

//add a frame between each frame
void addFrame(char *frameNo){

}



int main(int argc, char *argv[]){

	FILE *rleFile = fopen(argv[1], "r");

	if(rleFile == NULL){
		fprintf(stderr, "the file doesnt exist.");
		return 1;
	}

	Header rleFileHeader = {
		.width = 0,
		.height = 0
	};

	int result = headerDecode(rleFile, &rleFileHeader);
	if (!result) {
		return 1;
	}



	fclose(rleFile);
	if (strncmp(argv[2], "-", 1) == 0) {
		//each output file should be named 'prefix-xxxx.ppm', 'prefix' is the input
		//'xxxx' is a 4 digit integer indicating which frame it is
		
		return 0;
	} else if(strncmp(argv[3], "--scale", 7) == 0){
		
	} else if(strncmp(argv[3], "--tween", 7) == 0){
		
	} else {
		
	}
	return 0;
}



