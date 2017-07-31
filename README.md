# rledecode
## Author and Create time
	Tian Zhou
	28 March 2016


### Installation and Usage
	* open Terminal and change the directory to the folder of the program
	* simply type in 'make' to compile the program
	* commands to run the program: 
			* option1: (program name) (file name) -
			* option2: (program name) (file name) (prefix name)
			* option3 (optional): (program name) (file name) (prefix name)/'-' --tween (tweenfactor)
			* option4 (optional): (program name) (file name) (prefix name)/'-' --scale (scalefactor)
			* other options: you can choose to use --tween and --scale together since they are optional arguments

 
### What functonality is and is not supported

		* option1: (program name) (file name) -
			Example: rledecode sonic.rle - 
			Explanation: this will print out all the frames in the sonic.rle file with '-1'
			seperating each frame to Terminal.

		* option2: (program name) (file name) (prefix name)
			Example: rledecode sonic.rle sonic
			Explanation: this will generate a bunch of ppm files that are each frame per file named with 'sonic-0000.ppm', 'sonic-0001.ppm', 'sonic-0002.ppm' and so on. The number after the prefix name is the count of each frame.

		* option3 (optional): (program name) (file name) (prefix name)/'-' --tween (tweenfactor)
			Example: rledecode sonic.rle sonic --tween 2
			Explanation: this will generate a bunch of ppm files -- each frame per file, but 2 frames between each 2 frames to make the video more smooth.

		* option4 (optional): (program name) (file name) (prefix name)/'-' --scale (scalefactor)
			Example: rledecode sonic.rle sonic --scale 2
			Explanation: this will output a bunch of ppm files that are scaled by 2


### Notes
	* if --tween factor is too large you could run out of memory
