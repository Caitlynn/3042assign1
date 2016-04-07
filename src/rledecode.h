#ifndef DECODE_H
#define DECODE_H

typedef struct Arguments{
	char *video;
	char *prefix;
	int scaleNumber;
	int tweenFrame;

} Arguments;

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