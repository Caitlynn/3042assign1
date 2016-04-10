#include "rledecode.h"
#include "arguments/arguments.h"
#include <stdbool.h>
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

// int scaleImage(FILE *file, int scalefactor){
// 	if (fsanf(file, "P6\n%d %d\n255\n", width, height) != 2) {
// 		fprintf(stderr, "This is not a PPM file\n");
// 	}


// }

/*write a frame to a file*/
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



void writeToFile(int filecount, char *prefix, Frame *frame){
	if (prefix == NULL) {
			writeFile(frame, stdout);
			fprintf(stdout, "%d", -1);
		} else {
			char *filename = malloc(sizeof(char) * (4+4+1+strlen(prefix))); 	//allocate the momery of '-0000.ppm' and name of the prefix
			sprintf(filename, "%s-%04d.ppm", prefix, filecount); // generate filename 
			FILE *outputfile = fopen(filename, "wb"); //write to output file
			writeFile(frame, outputfile);
			fclose(outputfile);
			free(filename);
			//try to use the function so it can also write tween frames
			filecount++;
		}
}


int main(int argc, char *argv[]){
	//validate arguments numbers
	Arguments args = {
		.video = NULL,
		.scalefactor = 0,
		.tweenfactor = 0
	};
	if (!validateArguments(&args, argc, argv)) { // pass in the program arguments and fillout the Arguments struct
		return 1;
	}

	FILE *rleFile = fopen(args.video, "r");
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
	// an array to hold frames if tweening (only 2 frames!)
	Frame *twoArray[2] = {NULL, NULL};
	while((decoderesult = packbitDecode(rleFile, &frame)) != 0){
		// if tweening
		if (args.tweenfactor > 0){
			//copy the frame to temp
			Frame *temp = malloc(sizeof(Frame));
			temp->height = frame.height;
			temp->width = frame.width;
			size_t pixels = frame.height * frame.width;
			temp->framedata = malloc(sizeof(char) * pixels * 3);
			memcpy((void *)temp->framedata, (void *)frame.framedata, pixels * 3);

			if (filecount == 0){
				twoArray[0] = temp;
			} else if(filecount > 0){
				twoArray[1] = temp;
				Frame **tFrames = tweenFrame(twoArray[0],twoArray[1], args.tweenfactor);// if it's not the first frame, then generate the tweenFrames
				for(int i = 0; i<args.tweenfactor; i++){ // write the tween frames
					writeToFile(filecount++, args.prefix, tFrames[i]);
				}
				free(twoArray[0]);
				twoArray[0] = temp;
			}
		}
		writeToFile(filecount++, args.prefix, &frame);
	}

	free(frame.framedata);
	if (twoArray[0] != NULL) {
		free(twoArray[0]);
	}

	fclose(rleFile);

	return 0;
}


