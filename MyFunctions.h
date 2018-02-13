#ifndef _MYFUNCTIONS_H
#define _MYFUNCTIONS_H
#include<stdlib.h>
#include<sys/stat.h>


void DisplayArgs(char** args);


int ExistCheck(const char* filename);
int ExecCheck(const char* filename);
int CmdCheck(char** args, int a);
int CharCheck(const char* s, char c);
int StringCheck(char** argv, const char* s);


char* BPushString(char* x, char y);
char* FPushString(char* x, char y);
char** PBackArr(char** argv, const char* s);

char* RmDir(char* p);
char* DelFunc(char* line, size_t start, size_t end);
char* CharRep(char* x, size_t start, size_t end, const char* y);
char** RemoveArr(char** argv, int a);

char* PathMaker(char* s);
char* PathClear(char* s);
char* PathFromEVar(char* s);


int CmdVal(const char* s);
int GetSize(char** argv);
char* Convert(char** argv);
int CountStr(char** argv, const char* s);


char* LinkString(char* x, const char* y);

void MemFunc(char** arr);






#endif
