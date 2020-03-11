all:
	gcc -O2 -fPIC sunsensor.c -c -I./
	ld -shared sunsensor.o -o libsunsensor.so -lm
