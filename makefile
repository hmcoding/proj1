all: main.o MyParse.o MyProcesses.o MyShell.o MyFunctions.o  
	gcc -Wall -pedantic -std=c99 -g -o MyShell.x main.o MyParse.o MyProcesses.o MyShell.o MyFunctions.o 
	

main.o: main.c
	gcc -Wall -pedantic -std=c99 -g -c main.c



MyParse.o: MyParse.c MyParse.h
	gcc -Wall -pedantic -std=c99 -g -c MyParse.c

MyProcesses.o: MyProcesses.c MyProcesses.h
	gcc -Wall -pedantic -std=c99 -g -c MyProcesses.c


MyShell.o: MyShell.c MyShell.h
	gcc -Wall -pedantic -std=c99 -g -c MyShell.c

MyFunctions.o: MyFunctions.c MyFunctions.h
	gcc -Wall -pedantic -std=c99 -g -c MyFunctions.c

clean:
	rm *.o MyShell.x 
