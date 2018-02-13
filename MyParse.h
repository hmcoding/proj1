
#ifndef _MYPARSE_H
#define _MYPARSE_H


char* grabIn();


char** parseIn(char* inp);


char* whiteParse(char* line);


char** argsParse(char* inp);


char** getPaths(char** args);


char** expandVar(char** args);

#endif
