#include "rledecode.h"
#include <getopt.h>
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
	int pixel;
	if (feof(rleFile)){
		return 0;
	} else {
	//get the width and the height
	fscanf(rleFile, "RLEPLAYV1\n%d %d\n",&(header->width), &(header->height));
	pixel = (header->width)*(header->height);
	printf("%d\n", pixel);
	}

	if (header->width < 1 || header->height < 1){
		return 0;
	}
	return 1;
}

char packbitDecode(FILE *file){
	return '0';
}


// scale an image using blinear interpolation
void scaleImage(char *scaleRange,char *images){

}

//add a frame between each frame
void addFrame(char *frameNo){

}



int main(int argc, char *argv[]){

	FILE *rleFile = fopen(argv[1], "r");
	int *frameNo;
	if(rleFile == NULL){
		fprintf(stderr, "the file doesnt exist.");
		return 0;
	}

	Header rleFileHeader = {
		.width = 0,
		.height = 0
	};

	headerDecode(rleFile, &rleFileHeader);
	fclose(rleFile);
	if (strncmp(argv[2], "-", 1) == 0) {
		//each output file should be named 'prefix-xxxx.ppm', 'prefix' is the input
		//'xxxx' is a 4 digit integer indicating which frame it is
		printf("prefix- %d", *frameNo);
		return 0;
	} else if(strncmp(argv[3], "--scale", 7) == 0){
		
	} else if(strncmp(argv[3], "--tween", 7) == 0){
		
	} else {
		
	}
	return 0;
}



