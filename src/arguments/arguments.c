#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <stdlib.h>
#include "arguments.h"

/*this function validate the arguments and decide the actions of the arguments*/
int validateArguments(Arguments *args, int argc, char **argv) {
	if (argc < 3 || argc > 7){
		fprintf(stderr, "Please enter correct arugments\n");
		return false;
	}

	args->video = argv[1];
	FILE *file = fopen(args->video, "r");
	if (file == NULL) {
		fprintf(stderr, "could not open file '%s'\n", args->video);
		fclose(file);
		return false;
	}
	fclose(file);

	if(strncmp(argv[2],"-",1) ==0){
		args->prefix = NULL; //if it's a string, set prefix to null
	} else {
		args->prefix = argv[2]; //otherwise use the prefix name
	}

	const struct option long_options[] = {
		{"scale", 1, NULL,'s'},
		{"tween", 1, NULL,'t'},
	};
	int indexptr;
	int arg;
	//loop through the arguments
	while((arg = getopt_long(argc, argv, "s:t:", long_options, &indexptr)) != -1){
		switch(arg){
			//do this cases only if you need it
			case 's':
				args->scalefactor = atoi(optarg); //cast the argument to int and set scalefactor to the int
				break;
			case 't':
				args->tweenfactor = atoi(optarg);
				break;
			case ':':
				fprintf(stderr, "missing required parameter argument\n");
				return false;
		}
	}
	return true;
}