#ifndef _MYSHELL_H
#define _MYSHELL_H
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<sys/time.h>
#include <errno.h>
#include "MyFunctions.h"
#include "MyParse.h"
#include "MyProcesses.h"




void startMyShell();

void dirChange(const char* path);


void theExtern(char** argv, int back, char* cmd);

char** externIn(char** argv, int inp, int back);

char** externOut(char** argv, int outp, int back);

char** externPipe(char** argv, int numpipe, int back);


void handleIO(char** argv, int dir, char* filename, int back, char* cmd);

int errorsPipeIO(char** argv);

int errorsBackground(char** argv);

void displayPrompt();

void ioCmd(char** argv);

void etimeCmd(char** argv);

int checkZero(int tocheck);

void KillZombies();


#endif
