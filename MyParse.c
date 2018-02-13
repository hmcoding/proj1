#include "MyParse.h"
#include "MyFunctions.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

char* grabIn()
{
	size_t INSIZE = 512;
	char* inpStr = (char*)calloc(INSIZE, sizeof(char));
	if(fgets(inpStr, (int)INSIZE, stdin))
	{
		return inpStr;
	}
	else
	{
		return NULL;
	}
}

char** parseIn(char* inp)
{
	inp = whiteParse(inp);
	
	char** inpArgs = argsParse(inp);
	
	if (inpArgs[0] != NULL && (strcmp(inpArgs[0], "&") == 0))
	{
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
		if (hasit == 1)
		{
			if (numWspace > 0)
			{
				line = DelFunc(line, obj-numWspace-1, obj-2);
			}
			break;
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
		if ((check == ' ') || (check == '\n') || (check == '\t'))
		{
			numTok++;
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
	for (i = 1; i < numTok; i++)
	{
		temp = strtok(NULL, " \n\t");
		ans[i] = (char*)calloc(strlen(temp)+1, sizeof(char));
		strcpy(ans[i], temp);
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
		
		if (new == 1)
		{
			whatCmd = CmdCheck(args, itrArg);  
			cmd = args[itrArg];
			itrCmd = itrArg;
			new = 0;
		}
		else
		{
			if ((strcmp(args[itrArg], "|") == 0) || 
			    (strcmp(args[itrArg], "<") == 0) ||
			    (strcmp(args[itrArg], ">") == 0))
			    {
				new = 1;
				++itrArg;
				continue;
			    }
		}
		
		// cd
		if (whatCmd == 2)
		{
			


			if (itrArg == (itrCmd + 1))
			{
				if (!CharCheck(args[itrArg], '/'))
				{
					if (!CharCheck(args[itrArg], '~') && !CharCheck(args[itrArg], '.'))
					{
						args[itrArg] = FPushString(args[itrArg], '/');
						args[itrArg] = FPushString(args[itrArg], '.');
					}
				}
				else
				{
					if (args[itrArg][0] != '/' &&
						args[itrArg][0] != '.' &&
						args[itrArg][0] != '~')
					{
						args[itrArg] = FPushString(args[itrArg], '/');
						args[itrArg] = FPushString(args[itrArg], '.');
					}
				}
				args[itrArg] = PathMaker(args[itrArg]);
			}


		}

		
		else if (whatCmd == 3)
		{
			
			if ((strcmp(cmd, "etime") == 0) || (strcmp(cmd, "io") == 0))
			{
				
				if (itrArg == (itrCmd + 1))
				{
					if (CharCheck(args[itrArg], '/') == 1)
					{
						args[itrArg] = PathMaker(args[itrArg]);
					}
					else
					{
						args[itrArg] = PathFromEVar(args[itrArg]);
					}

					
				}
			}
		}

		
		else if (whatCmd == 1)
		{
			if (itrArg == itrCmd)
			{
				if (CharCheck(args[itrArg], '/') == 1)
				{
					args[itrArg] = PathMaker(args[itrArg]);
				}
				else
				{
					args[itrArg] = PathFromEVar(args[itrArg]);
				}
			}
		}
	
		
		++itrArg;
	}

	return args;
}

char** expandVar(char** args)
{
	size_t arg_it = 0;
	size_t str_it = 0;
	

	while (args[arg_it] != NULL)
	{
		char c = args[arg_it][str_it];

		while (c != '\0')
		{
			if (c == '$')
			{
				char* env_var = (char*)calloc(2, sizeof(char));
				size_t count = 1;
				c = args[arg_it][++str_it];
				if (c == '\0' || c == '$')
				{
					free(env_var);
					
					break;
				}
				env_var[0] = c;
				env_var[1] = '\0';
				c = args[arg_it][++str_it];
				while (c != '/' && c != '\0' && c != '$')
				{
					env_var = BPushString(env_var, c);
					c = args[arg_it][++str_it];
					count++;
				}
				char* ret_env = getenv(env_var);
				if (ret_env == NULL)
				{
					free(env_var);
					break;
				}
				args[arg_it] = CharRep(args[arg_it], str_it - count - 1, str_it - 1, ret_env);
				
				str_it = str_it + strlen(env_var);
				free(env_var);
			}
			c = args[arg_it][++str_it];
		}
		str_it = 0;
		++arg_it;
	}

	return args;
}
