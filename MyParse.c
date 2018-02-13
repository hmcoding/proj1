#include "MyParse.h"
#include "MyFunctions.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

char* grabIn()
{
	size_t BUFFER_SIZE = 512;
	char* input_string = (char*)calloc(BUFFER_SIZE, sizeof(char));
	if(fgets(input_string, (int)BUFFER_SIZE, stdin))
	{
		return input_string;
	}
	else
	{
		return NULL;
	}
}

char** parseIn(char* inp)
{
	inp = whiteParse(inp);
	
	char** split_args = argsParse(inp);
	
	if (split_args[0] != NULL)
	{
		if (strcmp(split_args[0], "&") == 0)
		{
			split_args = RemoveArr(split_args, 0);
		}
	}
	
	split_args = expandVar(split_args);
	
	split_args = getPaths(split_args);
	
	
	
	free(inp);
	
	return split_args;
}

char* whiteParse(char* line)
{
	size_t it = 0;
	int whitespace_count = 0;
	char cur_char = line[it];
	
	while (cur_char != '\0' && (cur_char == ' ' || cur_char == '\t' || cur_char == '\n'))
	{
		cur_char = line[++it];
		whitespace_count++;
	}
	if (it > 0)
		line = DelFunc(line, it-whitespace_count, it-1);
	
	
	
	it = 0;
	whitespace_count = 0;
	
	
	int contains_trailing = 0;
	
	while (cur_char != '\0')
	{
		cur_char = line[it++];
		while (cur_char == ' ' || cur_char == '\t' || cur_char == '\n')
		{
			whitespace_count++;
			cur_char = line[it++];
			if (cur_char == '\0')
				contains_trailing = 1;
		}
		if (contains_trailing == 1)
		{
			if (whitespace_count > 0)
			{
				line = DelFunc(line, it-whitespace_count-1, it-2);
			}
			break;
		}
		else if (whitespace_count > 1)
		{
			line = DelFunc(line, it-whitespace_count-1, it-3);
			// must update iterator if array is changed through deletion
			it = it - (whitespace_count - 1);
		}
		whitespace_count = 0;
	}
	
	return line;
}

char** argsParse(char* inp)
{
	size_t it = 0;
	char c = inp[it];
	int token_count = 1;

	while (c != '\0')
	{
		if ((c == ' ') || (c == '\n') || (c == '\t'))
		{
			token_count++;
		}
		c = inp[++it];
	}

	int i = 0;

	char** ret = (char**)calloc(token_count + 1, sizeof(char*));

	char* tmp = strtok(inp, " \n\t");

	if (tmp != NULL)
	{
		ret[i] = (char*)calloc(strlen(tmp)+1, sizeof(char));
		strcpy(ret[i], tmp);
	}
	for (i = 1; i < token_count; i++)
	{
		tmp = strtok(NULL, " \n\t");
		ret[i] = (char*)calloc(strlen(tmp)+1, sizeof(char));
		strcpy(ret[i], tmp);
	}

	ret[i] = NULL;

	return ret;
}

char** getPaths(char** args)
{
	int arg_it = 0;		
	int cmd_type = 0;	
	int cmd_it = 0;		
	int new_cmd = 1;	
	char* cur_cmd = args[0];


	while(args[arg_it] != NULL)
	{
		
		if (new_cmd == 1)
		{
			cmd_type = CmdCheck(args, arg_it);  
			cur_cmd = args[arg_it];
			cmd_it = arg_it;
			new_cmd = 0;
		}
		else
		{
			if ((strcmp(args[arg_it], "|") == 0) || 
			    (strcmp(args[arg_it], "<") == 0) ||
			    (strcmp(args[arg_it], ">") == 0))
			    {
				new_cmd = 1;
				++arg_it;
				continue;
			    }
		}
		
		// cd
		if (cmd_type == 2)
		{
			


			if (arg_it == (cmd_it + 1))
			{
				if (!CharCheck(args[arg_it], '/'))
				{
					if (!CharCheck(args[arg_it], '~') && !CharCheck(args[arg_it], '.'))
					{
						args[arg_it] = FPushString(args[arg_it], '/');
						args[arg_it] = FPushString(args[arg_it], '.');
					}
				}
				else
				{
					if (args[arg_it][0] != '/' &&
						args[arg_it][0] != '.' &&
						args[arg_it][0] != '~')
					{
						args[arg_it] = FPushString(args[arg_it], '/');
						args[arg_it] = FPushString(args[arg_it], '.');
					}
				}
				args[arg_it] = PathMaker(args[arg_it]);
			}


		}

		
		else if (cmd_type == 3)
		{
			
			if ((strcmp(cur_cmd, "etime") == 0) || (strcmp(cur_cmd, "io") == 0))
			{
				
				if (arg_it == (cmd_it + 1))
				{
					if (CharCheck(args[arg_it], '/') == 1)
					{
						args[arg_it] = PathMaker(args[arg_it]);
					}
					else
					{
						args[arg_it] = PathFromEVar(args[arg_it]);
					}

					
				}
			}
		}

		
		else if (cmd_type == 1)
		{
			if (arg_it == cmd_it)
			{
				if (CharCheck(args[arg_it], '/') == 1)
				{
					args[arg_it] = PathMaker(args[arg_it]);
				}
				else
				{
					args[arg_it] = PathFromEVar(args[arg_it]);
				}
			}
		}
	
		
		++arg_it;
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
