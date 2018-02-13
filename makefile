all: main.o parse.o utility.o shell.o background.o pipe.o 
	gcc -Wall -pedantic -std=c99 -g -o shell.x main.o parse.o utility.o shell.o background.o pipe.o
	

main.o: main.c
	gcc -Wall -pedantic -std=c99 -g -c main.c



parse.o: parse.c parse.h
	gcc -Wall -pedantic -std=c99 -g -c parse.c

utility.o: utility.c utility.h
	gcc -Wall -pedantic -std=c99 -g -c utility.c


shell.o: shell.c shell.h
	gcc -Wall -pedantic -std=c99 -g -c shell.c

background.o: background.c background.h
	gcc -Wall -pedantic -std=c99 -g -c background.c
	
pipe.o: pipe.c pipe.h
	gcc -Wall -pedantic -std=c99 -g -c pipe.c

clean:
	rm *.o shell.x test.x
