main: main.o print.o
	        gcc -o main main.o print.o
main.o: main.c print.h
	        gcc -c main.c
print.o: print.c print.h
	        gcc -c print.c  
