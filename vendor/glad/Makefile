all: glad.c
	gcc -no-pie -c glad.c -o glad.o 
	ar rcs libglad.a glad.o

clean:
	rm glad.o libglad.a