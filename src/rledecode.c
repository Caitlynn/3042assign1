#include "rledecode.h"
#include <stdbool.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/* this program decodes an rleplay file and produce a sequence of images in PPM format
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
	//handle incorrect width and height
	if (header->width < 1 || header->height < 1){
		fprintf(stderr, "the width and height values are incorrect\n");
		return false;
	}
	return true;
}

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

int writeFile(Frame *frame, FILE *framefile){
	fprintf(framefile, "P6\n%d %d\n255\n", frame->width, frame->height);
	int pixels = frame->width * frame->height;
	for(size_t i=0; i<pixels; i++){
		size_t offset = pixels;
		char pixel[3] = {
			frame->framedata[i],
			frame->framedata[i+offset],
			frame->framedata[i+2*offset]
		};
		if(fwrite(pixel, sizeof(char), 3, framefile) != 3) {
			return false;
		}
	}
	return true;	 	
}


int main(int argc, char *argv[]){
	//validate arguments numbers
	if (argc < 3 || argc > 7){
		fprintf(stderr, "Please enter correct arugments\n");
		return false;
	}

	Arguments args = {
		.video = argv[1],
		.scaleNumber = 0,
		.tweenFrame = 0
	};

	if(strncmp(argv[2],"-",1) ==0){
		args.prefix = NULL;
	} else {
		args.prefix = argv[2];
	}

	// const struct option long_options[]{
	// 	{"scale", 1, NULL,'s'},
	// 	{"tween", 1, NULL,'t'}
	// };
	// int indexptr;
	// //loop through the arguments
	// while((option = getopt_long(argc, argv, "s:t:", long_options, &indexptr)) != -1){
	// 	switch(option){
	// 		//do this cases only if you need it
	// 		case 's':
	// 			args.scaleNumber = atoi(optarg);

	// 		case 't':
	// 			args.tweenFrame = atoi(optarg);
	// 	}
	// }
	FILE *rleFile = fopen(argv[1], "r");

	if(rleFile == NULL){
		fprintf(stderr, "the file doesnt exist.");
		return false;
	}

	Header rleFileHeader = {
		.width = 0,
		.height = 0
	};

	int result = headerDecode(rleFile, &rleFileHeader);
	if (!result) {
		return 1;
	}

	Frame frame = {
		.height = rleFileHeader.height,
		.width = rleFileHeader.width,
		.framedata = NULL
	};

	int decoderesult = 0;
	int filecount = 0;
	while((decoderesult = packbitDecode(rleFile, &frame)) != 0){
		if (args.prefix == NULL) {
			writeFile(&frame, stdout);
			fprintf(stdout, "%d", -1);
		} else {
			//allocate the momery of '-0000.ppm' and name of the prefix
			char *filename = malloc(sizeof(char) * (4+4+1+strlen(args.prefix))); 
			sprintf(filename, "%s-%04d.ppm",args.prefix, filecount); // generate filename 
			FILE *outputfile = fopen(filename, "wb"); //write to output file
			writeFile(&frame, outputfile);
			free(filename);
			free(frame.framedata);
			filecount++;
		}

	}

	fclose(rleFile);

	return 0;
}



