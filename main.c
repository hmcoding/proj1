#include "parse.h"
#include "utility.h"
#include "shell.h"
#include<stdio.h>

int main()
{
	currentDirectory = (char*) calloc(1024, sizeof(char));
	RunShell();
	return 0;
}
