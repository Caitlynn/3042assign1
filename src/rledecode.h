#ifndef DECODE_H
#define DECODE_H

/* this is the struct for a frame with height width and all the framedata in a frame
*/
typedef struct Frame{
	unsigned int height;
	unsigned int width;
	unsigned char *framedata;
} Frame;

#endif 