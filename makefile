src = ./src
lib = ./lib
bin = ./bin
include = ./include

server: main.o serverlib.o
	gcc -o server main.o serverlib.o -lm
	mv server $(bin)
	mv *.o $(lib)
	
main.o : $(src)/main.c $(src)/serverlib.o
	gcc -g -Wall -c $(src)/main.c

serverlib.o : $(include)/serverlib.h $(src)/serverlib.c
	gcc -g -Wall -c $(src)/serverlib.c

clean :
	rm -f $(bin)/server $(lib)/*.o
	