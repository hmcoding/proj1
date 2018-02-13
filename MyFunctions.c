#include "MyFunctions.h"
#include<stdio.h>
#include<string.h>
#include<unistd.h>


void MemFunc(char** arr)
{
	size_t obj = 0; 
	
	while (arr[obj] != NULL)
	{
			free(arr[obj]); 
			++obj;
	}


	free(arr);
}

void DisplayArgs(char** args)
{
	size_t obj = 0;
	
	while (args[obj] != NULL)
	{
			printf(args[obj]);
			printf("\n"); 
			++obj;
	}
}

char* DelFunc(char* line, size_t start, size_t end)
{
	

	size_t charAmount = end - start + 1;
	int length = strlen(line)+1;
	int lengthUpdate = length - (int)charAmount;
	char* lineUpdate = (char*)calloc(lengthUpdate, sizeof(char)); 
	
	lineUpdate[lengthUpdate - 1] = '\0';
	size_t obj1 = 0;
	size_t obj2 = 0;
	char theChar = line[obj1];
	while (theChar != '\0')
	{
		if (!((obj1 >= start) && (obj1 <= end)))
		{
				lineUpdate[obj2++] = theChar;
		}
		theChar = line[++obj1];
	}
	free(line);
	line = NULL;
	return lineUpdate;
}

int ExistCheck(const char* filename)
{
	if (access(filename, F_OK) != -1) 
		return 1;
	else
		return 0;
}

int ExecCheck(const char* filename)
{
	struct stat s = {0}; 
	stat(filename, &s);
	int ans = S_ISREG(s.st_mode); 
	return ans;
}

char* LinkString(char* x, const char* y)
{
	size_t lengthTot = strlen(x) + strlen(y) + 1;
	char* stringUpdate = (char*)calloc(lengthTot, sizeof(char));
	int obj1 = strlen(x);
	int obj2 = 0;
	char c = x[obj2];

	while(c != '\0')
	{
		stringUpdate[obj2++] = c;
		c = x[obj2];
	}

	
	obj2 = 0;
	c = y[obj2];
	while (c != '\0')
	{
		stringUpdate[obj1++] = c;
		c = y[++obj2];
	}

	stringUpdate[obj1] = '\0';
	free(x);
	return stringUpdate;
}

char* BPushString(char* x, char y)
{
	size_t lengthTot = strlen(x) + 2;
	char* stringUpdate = (char*)calloc(lengthTot, sizeof(char));
	strcpy(stringUpdate, x);
	stringUpdate[lengthTot-2] = y;
	stringUpdate[lengthTot-1] = '\0';
	free(x);
	return stringUpdate;
}

char* FPushString(char* x, char y)
{
	size_t lengthTot = strlen(x) + 2;
	char* stringUpdate = (char*)calloc(lengthTot, sizeof(char));
	strcpy(stringUpdate, x);
	stringUpdate[0] = y;
	size_t obj = 0;
	while(x[obj] != '\0')
	{
		stringUpdate[obj+1] = x[obj];
		++obj;
	}
	stringUpdate[obj+1] = '\0';
	free(x);
	return stringUpdate;
}

char* RmDir(char* p)
{
	int toRem = strlen(p) - 1;
	int endRem = toRem;
	char c = p[toRem];

	

	if (c == '/' && toRem == 0)
		return p;
	
	
	if (c == '/')
	{
		c = p[--toRem];
	}

	
	
	while (c != '/')
	{
		c = p[--toRem];
	}

	return DelFunc(p, toRem, endRem);
}

int CmdCheck(char** args, int a)
{
	static char* cmdsInclude[5] = {"exit", "echo", "etime", "limits", NULL}; 

	if ((strcmp(args[a], "cd") != 0))
	{
		
		if (CmdVal(args[a]))
			return 1;
	}
	
	
	else if (strcmp(args[a], "cd") == 0)
		return 2;

	size_t obj = 0;
	char* temp = cmdsInclude[obj];
	
	while (temp != NULL)
	{
		
		if (strcmp(args[a], temp) == 0)
		{
			
			return 3;
		}
		temp = cmdsInclude[++obj];
		
	}
	
	return 0;

}	

char* PathMaker(char* s)
{
	char* stringAns = s;

	if ((strlen(stringAns) == 1) && (strcmp(stringAns, "/") == 0))
		return stringAns;

	
	if (stringAns[0] == '~')
	{
		stringAns = CharRep(stringAns, 0, 0, getenv("HOME")); 
	}

	
	if (stringAns[0] == '.' && stringAns[1] != '.')
	{
		
		if (strcmp(getenv("PWD"), "/") == 0)
			stringAns = CharRep(stringAns, 0, 1, getenv("PWD"));
		
		else
			stringAns = CharRep(stringAns, 0, 0, getenv("PWD"));
	}

	
	if (stringAns[0] == '.' && stringAns[1] == '.')
	{
		char* pwd = getenv("PWD");
		char* pwdNew = (char*)calloc(strlen(pwd)+1,sizeof(char));
		strcpy(pwdNew, pwd);
		pwdNew = RmDir(pwdNew);
		
		if (strcmp(pwdNew,"")==0)
		{
			stringAns = CharRep(stringAns, 0, strlen(stringAns)-1, "/");
			free(pwdNew);
			return stringAns;
		}
		stringAns = CharRep(stringAns, 0, 1, pwdNew);
		free(pwdNew);
	}

	
	size_t obj = 1;
	size_t slash1 = 0;
	size_t slash2 = 0;
	while (stringAns[obj] != '\0')
	{
		if (stringAns[obj] == '.' && stringAns[obj-1] == '.')
		{
			
			if (obj > 2)
			{
				stringAns = DelFunc(stringAns, slash2, obj); 
				
				obj = 0;
				slash1 = 0;
				slash2 = 0;
				
				if (strcmp(stringAns,"") == 0)
				{
					stringAns = BPushString(stringAns, '/');
					return stringAns;
				}
			}
			else if (strlen(stringAns) > 3)
			{
				stringAns = DelFunc(stringAns, obj-2, obj);
				obj = obj - 3;	
			}
			else
			{
				stringAns = DelFunc(stringAns, obj-1, obj);
				obj = obj - 2;
			}
		}
		
		else if ((strlen(stringAns) >= 2) && stringAns[obj-1] == '.' && stringAns[obj] == '/')
		{
			stringAns = DelFunc(stringAns, obj-1, obj);
			obj = obj - 2;
		}
		else if (stringAns[obj] == '/')
		{
			slash2 = slash1;
			slash1 = obj;
		}
		++obj;
	}

	return stringAns;
}

char* PathFromEVar(char* s)
{
	
	char* sNew = (char*)calloc(strlen(s)+2, sizeof(char));
	strcpy(sNew, "/");
	sNew = LinkString(sNew, s);

	char* p = getenv("PATH");	
	
	char* pNew = (char*)calloc(strlen(p)+1,sizeof(char));
	strcpy(pNew, p);
	char* temp = strtok(pNew, ":");

	while (temp != NULL)
	{	
		char* ThePath = (char*)calloc(strlen(temp)+1, sizeof(char));
		strcpy(ThePath, temp);

		ThePath = PathClear(ThePath);

		ThePath = LinkString(ThePath, sNew);
		ThePath = PathMaker(ThePath);
		
		if (ExecCheck(ThePath))
		{
			free(s);
			free(sNew);
			free(pNew);
			return ThePath;
		}
		free(ThePath);
		temp = strtok(NULL, ":");
	}

	free(pNew);
	free(sNew);
	return s;

}

int CmdVal(const char* s)
{
	
	char* sNew = (char*)calloc(strlen(s)+2, sizeof(char));
	strcpy(sNew, "/");
	sNew = LinkString(sNew, s);

	
	
	char* p = getenv("PATH");	
	char* pNew = (char*)calloc(strlen(p)+1,sizeof(char));
	strcpy(pNew, p);

	char* temp = strtok(pNew, ":");

	while (temp != NULL)
	{	
		char* ThePath = (char*)calloc(strlen(temp)+1, sizeof(char));
		strcpy(ThePath, temp);

		ThePath = PathClear(ThePath);

		ThePath = LinkString(ThePath, sNew);
		ThePath = PathMaker(ThePath);

		
		if (ExecCheck(ThePath))
		{
			free(ThePath);
			free(sNew);
			free(pNew);
			return 1;
		}
		free(ThePath);
		temp = strtok(NULL, ":");
	}
	free(pNew);
	free(sNew);
	return 0;
}

char* PathClear(char* s)
{
	size_t objLast = strlen(s) - 1;
	if (s[objLast] == '/')
	{
		s = DelFunc(s, objLast, objLast);
	}
	return s;
}

char* CharRep(char* x, size_t start, size_t end, const char* y)
{
	size_t lengthDel = end - start + 1;
	size_t xLength = strlen(x);
	size_t yLength = strlen(y);
	size_t lengthNew = xLength - lengthDel + yLength;

	char* stringAns = (char*)calloc(lengthNew + 1, sizeof(char));
	stringAns[lengthNew - 1] = '\0'; 

	int stringAnsObj = 0;
	int xObj = 0;
	int yObj = 0;
	int doneObj = 0;

	if (start == 0)
		doneObj = end + 1;
	else
		doneObj = end + 1;

	for (stringAnsObj = 0; stringAnsObj < lengthNew; stringAnsObj++)
	{
		if (stringAnsObj < start)
			stringAns[stringAnsObj] = x[xObj++];
		else if ((stringAnsObj >= start) && (stringAnsObj <= start+ yLength -1))
			stringAns[stringAnsObj] = y[yObj++];
		else
			stringAns[stringAnsObj] = x[doneObj++];
	}
	free(x);
	return stringAns;
}

int CharCheck(const char* s, char c)
{
	size_t obj = 0;
	while (s[obj] != '\0')
	{
		if (s[obj] == c)
		{
			return 1;
		}
		++obj;
	}
	return 0;
}

int StringCheck(char** argv, const char* s)
{
	int obj = 0;
	while (argv[obj] != NULL)
	{
		if (strcmp(argv[obj], s) == 0)
		{
			return obj;
		}
		++obj;
	}
	return -1;
}

int GetSize(char** argv)
{
	if (argv == NULL)
		return 0;
	int obj = 0;
	while(argv[obj] != NULL)
	{
		++obj;
	}
	return obj;
}

char** PBackArr(char** argv, const char* s)
{
	char** vecNew = (char**)calloc(GetSize(argv)+2,sizeof(char*));
	int obj = 0;
	while (argv[obj] != NULL)
	{
		vecNew[obj] = (char*)calloc(strlen(argv[obj])+1, sizeof(char));
		strcpy(vecNew[obj], argv[obj]);
		++obj;
	}
	vecNew[obj] = (char*)calloc(strlen(s)+1, sizeof(char));
	strcpy(vecNew[obj], s);
	vecNew[obj+1] = NULL;
	MemFunc(argv);
	return vecNew;
}

char** RemoveArr(char** argv, int a)
{
	if (a > GetSize(argv))
	{
		return argv;
	}
	int obj1 = 0;
	int obj2 = 0;

	char** vecNew = (char**)calloc(GetSize(argv),sizeof(char*));
	while (argv[obj1] != NULL)
	{
		if (obj1 != a)
		{
			vecNew[obj2] = (char*)calloc(strlen(argv[obj1])+1, sizeof(char));
			strcpy(vecNew[obj2], argv[obj1]);
			++obj2;
		}
		++obj1;
	}
	vecNew[obj2] = NULL;
	MemFunc(argv);
	return vecNew;
}

char* Convert(char** argv)
{
	int obj = 0;
	
	if (argv[obj] == NULL)
		return NULL;
	char* ans = (char*)calloc(1, sizeof(char));
	ans[0] = '\0';
	while (argv[obj] != NULL)
	{
		ans = LinkString(ans, argv[obj]);
		++obj;
		if (argv[obj] != NULL)	
			ans = BPushString(ans, ' ');
	}
	return ans;
}

int CountStr(char** argv, const char* s)
{
	int obj = 0;
	int track = 0;
	while (argv[obj] != NULL)
	{
		if (strcmp(argv[obj], s) == 0)
		{
			++track;
		}
		++obj;
	}
	return track;
}


