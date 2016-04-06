program=rledecode
flags=-Wall
src=$(shell find src -name "*.c")
obj=$(src:.c=.o)

all: $(program)

debug: $(eval flag := -g -Wall) $(program)

$(program): $(obj)
	gcc -o $@ $^

%.o: %.c %.h
	gcc -c $< -o $@ $(flags)

%.o: %.c
	gcc -c $< -o $@ $(flags)

clean:
	@find src/ -type f -name '*.o' -delete
	