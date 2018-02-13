#include "MyParse.h"
#include "MyFunctions.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

char* grabIn()
{
	size_t INSIZE = 512;
	char* inpStr = (char*)calloc(INSIZE, sizeof(char));
	
	if(!fgets(inpStr, (int)INSIZE, stdin))
		return NULL;
	else
		return inpStr;
}

char** parseIn(char* inp)
{
	inp = whiteParse(inp);
	
	char** inpArgs = argsParse(inp);
	
	if (inpArgs[0] != NULL)
	{
		if((strcmp(inpArgs[0], "&") == 0))
			inpArgs = RemoveArr(inpArgs, 0);
	}
		
	
	inpArgs = expandVar(inpArgs);
	
	inpArgs = getPaths(inpArgs);
	
	free(inp);
	
	return inpArgs;
}

char* whiteParse(char* line)
{
	size_t obj = 0;
	int numWspace = 0;
	char theChar = line[obj];
	
	while (theChar != '\0' && (theChar == ' ' || theChar == '\t' || theChar == '\n'))
	{
		theChar = line[++obj];
		numWspace++;
	}
	if (obj > 0)
		line = DelFunc(line, obj-numWspace, obj-1);
	
	
	
	obj = 0;
	numWspace = 0;
	
	
	int hasit = 0;
	
	while (theChar != '\0')
	{
		theChar = line[obj++];
		while (theChar == ' ' || theChar == '\t' || theChar == '\n')
		{
			numWspace++;
			theChar = line[obj++];
			if (theChar == '\0')
				hasit = 1;
		}
		//if (hasit == 1 && numWspace > 0)
		if (hasit == 1)
		{
			if(numWspace > 0)
			{
			line = DelFunc(line, obj-numWspace-1, obj-2);
			break;
			}
		}
		else if (numWspace > 1)
		{
			line = DelFunc(line, obj-numWspace-1, obj-3);
			
			obj = obj - (numWspace - 1);
		}
		numWspace = 0;
	}
	
	return line;
}

char** argsParse(char* inp)
{
	size_t obj = 0;
	char check = inp[obj];
	int numTok = 1;

	while (check != '\0')
	{
		switch(check){
			case ' ':
				numTok++;
				break;
			case '\n':
				numTok++;
				break;
			case '\t':
				numTok++;
				break;
		}
		
		check = inp[++obj];
	}

	int i = 0;

	char** ans = (char**)calloc(numTok + 1, sizeof(char*));

	char* temp = strtok(inp, " \n\t");

	if (temp != NULL)
	{
		ans[i] = (char*)calloc(strlen(temp)+1, sizeof(char));
		strcpy(ans[i], temp);
	}
	i = 1;
	while (i < numTok)
	{
		temp = strtok(NULL, " \n\t");
		ans[i] = (char*)calloc(strlen(temp)+1, sizeof(char));
		strcpy(ans[i], temp);
		i++;
	}

	ans[i] = NULL;

	return ans;
}

char** getPaths(char** args)
{
	int itrArg = 0;		
	int whatCmd = 0;	
	int itrCmd = 0;		
	int new = 1;	
	char* cmd;
	cmd = args[0];


	while(args[itrArg] != NULL)
	{
		
		switch(new){
			case 1:
			{
			whatCmd = CmdCheck(args, itrArg);  
			cmd = args[itrArg];
			itrCmd = itrArg;
			new = 0;
			break;
			}
			default:
			{
			if ((strcmp(args[itrArg], "|") == 0) || 
			    (strcmp(args[itrArg], "<") == 0) ||
			    (strcmp(args[itrArg], ">") == 0))
			    {
				new = 1;
				++itrArg;
				continue;
			    }
				break;
			}
		}
		
	
		switch(whatCmd)
		{
			case 2:		
			{
			


			if (itrArg == (itrCmd + 1))
			  {
				if (!CharCheck(args[itrArg], '/'))
				{
					if (!CharCheck(args[itrArg], '~'))
					{
						if (!CharCheck(args[itrArg], '.'))
						{
						args[itrArg] = FPushString(args[itrArg], '/');
						args[itrArg] = FPushString(args[itrArg], '.');
						}
					}
				}
				else
				{
					if (args[itrArg][0] != '/')
					{
						if(args[itrArg][0] != '.')
						{
							if(args[itrArg][0] != '~')
							{	
							args[itrArg] = FPushString(args[itrArg], '/');
							args[itrArg] = FPushString(args[itrArg], '.');
							}
						}
					}
				}
				args[itrArg] = PathMaker(args[itrArg]);
			  }
				break;


			}

		
			case 3:
			{
			
			if ((strcmp(cmd, "etime") == 0) || (strcmp(cmd, "io") == 0))
			  {
				
				if (itrArg == (itrCmd + 1))
				{
					
					if (CharCheck(args[itrArg], '/') != 1)
					{
						args[itrArg] = PathFromEVar(args[itrArg]);
					}
					else
					{
						args[itrArg] = PathMaker(args[itrArg]);
					}

					
				}
			  }
				break;
			}

		
			case 1:
			{
			if (itrArg == itrCmd)
			  {
				
				if (CharCheck(args[itrArg], '/') != 1)
				{
					args[itrArg] = PathFromEVar(args[itrArg]);
				}
				else
				{
					args[itrArg] = PathMaker(args[itrArg]);
				}
						 
				
			  }
				break;
			}
	}
	
		
		++itrArg;
	}

	return args;
}

char** expandVar(char** args)
{
	size_t itrArg = 0;
	size_t itrStr = 0;
	

	while (args[itrArg] != NULL)
	{
		char check = args[itrArg][itrStr];

		while (check != '\0')
		{
			switch(check)
			{
				case '$':
			  {
				char* eVar = (char*)calloc(2, sizeof(char));
				size_t track = 1;
				check = args[itrArg][++itrStr];
				if (check == '\0' || check == '$')
				{
					free(eVar);
					
					break;
				}
				eVar[0] = check;
				eVar[1] = '\0';
				check = args[itrArg][++itrStr];
				while (check != '/' && check != '\0' && check != '$')
				{
					eVar = BPushString(eVar, check);
					check = args[itrArg][++itrStr];
					track++;
				}
				char* eAns = getenv(eVar);
				if (eAns == NULL)
				{
					free(eVar);
					break;
				}
				args[itrArg] = CharRep(args[itrArg], itrStr - track - 1, itrStr - 1, eAns);
				
				itrStr = itrStr + strlen(eVar);
				free(eVar);
			  }
			}
			check = args[itrArg][++itrStr];
			
		  }
		itrStr = 0;
		++itrArg;
	}

	return args;
}
