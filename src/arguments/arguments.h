#ifndef ARGUMENTS_H
#define ARGUMENTS_H

typedef struct Arguments{
	char *video;
	char *prefix;
	int scalefactor;
	int tweenfactor;
} Arguments;

int validateArguments(Arguments *args, int argc, char **argv);

#endif