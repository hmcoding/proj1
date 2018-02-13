#include "shell.h"



void startMyShell()
{
	int started = 1;
	char* str;
	char** argv;
	Init();
	while (started)
	{
		

		checkQueue();
		displayPrompt();
		str = ReadInput();
		argv = ParseInput(str);
		
		if ((argv[0] == NULL) || errorsPipeIO(argv) || errorsBackground(argv))
		{
			
		}
	
		else if (strcmp(argv[0], "cd") == 0)
		{
			
			if (GetSize(argv) == 2)
				dirChange(argv[1]);
			else if (GetSize(argv) < 2)
				dirChange(getenv("HOME"));
			else
				printf("There are too many arguments\n");
			

		}
		else if (strcmp(argv[0], "etime") == 0)
		{
			if (GetSize(argv) > 1)
			{
				argv = RemoveArr(argv, 0);
				etimeCmd(argv);
			}
		}

		else if (strcmp(argv[0], "io") == 0)
		{
			if (GetSize(argv) > 1)
			{
				argv = RemoveArr(argv, 0);
				ioCmd(argv);
			}

		}
		else if (strcmp(argv[0], "exit") == 0)
		{
			MemFunc(argv);
			printf("Exiting Shell...\n");
			KillZombies();
			exitQueue();
			exit(0);
		}
		
		else if (ExecCheck(argv[0]))
		{
			int back = StringCheck(argv, "&");
			int inp = StringCheck(argv, "<");
			int outp = StringCheck(argv, ">");
			int numpipe = CountStr(argv, "|");
			if (inp != -1)
			{
				argv = externIn(argv, inp, back);
			}
			else if (outp != -1)
			{
				argv = externOut(argv, outp, back);
			}
			else if (numpipe > 0)
			{
				argv = externPipe(argv, numpipe, back);
			}
			else
			{
				char* cmd = Convert(argv);
				if (back != -1)
				{
					argv = RemoveArr(argv, back);
				}
				theExtern(argv, back, cmd);
				free(cmd);
			}
		}


		
		MemFunc(argv);
	}
}

void dirChange(const char* dir)
{

	int ans = chdir(dir);
	switch(ans){
	case 0:
	{
		setenv("PWD", dir, 1);
		break;
	}
	default:
	{
		printf("This directory does not exist\n");
		break;
	}
	}

}



void theExtern(char** argv, int back, char* cmd)
{
	int now;
	pid_t pid = fork();
	
	switch(pid){
	case -1:
	{
		printf("Fork is causing an error\n");
		exit(1);
		break;
	}
	case 0:
	{
		execv(argv[0], argv);
		printf("Not able to execute: \n");
		DisplayArgs(argv);
		exit(1);
		break;
	}
	default:
	{
		
		switch(back){
		default:
		{
			
			waitpid(pid, &now, WNOHANG);
			
			handleQueue(newPro(pid, -1, cmd));
			
			
			break;
		}
		case -1:
		{
			
			waitpid(pid, &now, 0);
			break;
		}
		}
		break;
	}
	}
}

char** externIn(char** argv, int inp, int back)
{
	char* filename = (char*)calloc(strlen(argv[inp+1])+1, sizeof(char));
	strcpy(filename, argv[inp+1]);

	argv = RemoveArr(argv, inp);
	argv = RemoveArr(argv, inp);
				
	back = StringCheck(argv, "&");
	char* cmd = Convert(argv);
	
	
	switch(back){
		case -1:
			break;
		default:
			argv = RemoveArr(argv, back);
			break;
	}
	
	
	handleIO(argv, 1, filename, back, cmd);
	free(filename);
	free(cmd);
	
	return argv;
}

char** externOut(char** argv, int outp, int back)
{
	char* filename = (char*)calloc(strlen(argv[outp+1])+1, sizeof(char));
	strcpy(filename, argv[outp+1]);

	argv = RemoveArr(argv, outp);
	argv = RemoveArr(argv, outp);
			
	// update background iterator
	back = StringCheck(argv, "&");
	char* cmd = Convert(argv);
	switch(back){
		case -1:
			break;
		default:
			argv = RemoveArr(argv, back);
			break;
	}
	handleIO(argv, 0, filename, back, cmd);
	free(filename);
	free(cmd);
	
	return argv;
}

char** externPipe(char** argv, int numpipe, int back)
{
	char* cmd = Convert(argv);
	switch(back){
		case -1:
			break;
		default:
			argv = RemoveArr(argv, back);
			break;
	}

	if (numpipe == 1 || numpipe == 2 || numpipe == 3)
{
    char** argv1 = (char**)calloc(1, sizeof(char*));
    char** argv2 = (char**)calloc(1, sizeof(char*));
    char** argv3 = (char**)calloc(1, sizeof(char*));
    char** argv4 = (char**)calloc(1, sizeof(char*));
    argv1[0] = NULL;
    argv2[0] = NULL;
    argv3[0] = NULL;
    argv4[0] = NULL;
    int it = 0;
    while(strcmp(argv[it], "|") != 0)
    {
        argv1 = PBackArr(argv1, argv[it]);
        ++it;
    }
    ++it;
        if (numpipe == 1)
        {
            while(argv[it] != NULL)
            {
                argv2 = PBackArr(argv2, argv[it]);
                ++it;
            }
            OnePipe(argv1, argv2, back, cmd);
            MemFunc(argv1);
            MemFunc(argv2);
        }
        else if (numpipe == 2 )
        {
            while(strcmp(argv[it], "|") != 0)
            {
                argv2 = PBackArr(argv2, argv[it]);
                ++it;
            }
            ++it;
            while(argv[it] != NULL)
            {
                argv3 = PBackArr(argv3, argv[it]);
                ++it;
            }
            TwoPipe(argv1, argv2, argv3, back, cmd);
            MemFunc(argv1);
            MemFunc(argv2);
            MemFunc(argv3);
        }
        else if (numpipe == 3)
        {
            while(strcmp(argv[it], "|") != 0)
            {
                argv2 = PBackArr(argv2, argv[it]);
                ++it;
            }
            ++it;
            while(strcmp(argv[it], "|") != 0)
            {
                argv3 = PBackArr(argv3, argv[it]);
                ++it;
            }
            ++it;
            while(argv[it] != NULL)
            {
                argv4 = PBackArr(argv4, argv[it]);
                ++it;
            }
            ThreePipe(argv1, argv2, argv3, argv4, back, cmd);
            MemFunc(argv1);
            MemFunc(argv2);
            MemFunc(argv3);
            MemFunc(argv4);
        }
}	
	free(cmd);
	return argv;





}

void handleIO(char** argv, int directory, char* filename, int back, char* cmd)
{
	// output redirection
	switch(directory){
	case 0:
	{
		int now;
		int fIO = open(filename, O_CREAT|O_WRONLY|O_TRUNC, 0777);
		if (fIO == -1)
		{
			printf("Cannot open the file filename: %s\n", filename);
			exit(1);
		}
		pid_t pid = fork();

		switch(checkZero(pid)){
		case 2:
		{
			close(1);
			dup(fIO);
			close(fIO);
			execv(argv[0], argv);
			printf("Command invalid\n");
			exit(1);
			break;
		}
		case 1:
		{
			switch(back){
			default:
			{
				waitpid(pid, &now, WNOHANG);
				handleQueue(newPro(pid, -1, cmd));
				break;
			}
			case -1:
			{
				waitpid(pid, &now, 0);
				break;
			}
			}
			close(fIO);
			break;
		}
		case 0:
		{
			printf("The fork has failed.\n");
			exit(1);
			break;
		}
		}
		break;
	}
	case 1:
	{
		int now;
		int fIO = open(filename, O_RDONLY);
		switch(fIO){
              	  case -1:
		  {
			printf("Cannot access file\n");
			exit(1);
			break;
		  }
		}
		pid_t pid = fork();
		
		switch(checkZero(pid)){
		case 2:
		{
			close(0);
			dup(fIO);
			close(fIO);
			execv(argv[0], argv);

			printf("Command invalid\n");
			exit(1);
			break;
		}
		case 1:
		{
		
			switch(back){
			default:
			{
				waitpid(pid, &now, WNOHANG);
				handleQueue(newPro(pid, -1, cmd));
				break;
			}
			case -1:
			{
				waitpid(pid, &now, 0);
				break;
			}
			}
			close(fIO);
			break;
		}
		case 0:
		{
			printf("fork() has failed\n");
			exit(1);
			break;
		}
		}
		break;
	}
	}
}

int errorsPipeIO(char** argv)
{
	
	switch(GetSize(argv)){
	case 1:
	{
		if ((StringCheck(argv, "<") != -1) ||
			(StringCheck(argv, ">") != -1) ||
			(StringCheck(argv, "|") != -1))
		{
			printf("Need greater than 1 argument\n");
			return 1;
		}
		break;
	}
	default:
	{
		if ((StringCheck(argv, "<") == 0) ||
			(StringCheck(argv, ">") == 0) ||
			(StringCheck(argv, "|") == 0))
		{
			printf("No IO redirection or pipe allowed at beginning of statement\n");
			return 1;
		}
		else if ((StringCheck(argv, "<") == GetSize(argv)-1) ||
				 (StringCheck(argv, ">") == GetSize(argv)-1) ||
				 (StringCheck(argv, "|") == GetSize(argv)-1))
		{
			printf("No IO redirection or pipe allowed at end of statement\n");
			return 1;
		}
		break;
	}
	}
	return 0;
}

int errorsBackground(char** argv)
{
	int inp = StringCheck(argv, "<");
	int outp = StringCheck(argv, ">");
	int thepipe = StringCheck(argv, "|");
	int theback = StringCheck(argv, "&");
	
	if (theback == -1)
	{
		return 0;
	}
	
	// if not < > or |, no error
	if ((inp == -1) &&
		(outp == -1) &&
		(thepipe == -1))
	{
		return 0;
	}
	else
	{
		if (inp != -1)
		{
			// & neighbors <
			if (inp - 1 == theback || inp + 1 == theback)
			{
				printf("Incorrect format for executing background processing\n");
				return 1;
			}
		}
		if (outp != -1)
		{
			// & neighbors >
			if (outp - 1 == theback || outp + 1 == theback)
			{
				printf("Incorrect format for executing background processing\n");
				return 1;
			}
		}
		if (thepipe != -1)
		{
			// & neighbors |
			if (thepipe - 1 == theback || thepipe + 1 == theback)
			{
				printf("Incorrect format for executing background processing\n");
				return 1;
			}
		}
	}
	return 0;
}

void displayPrompt()
{
	const char* workDir = "PWD";
	const char* mach = "MACHINE";
	const char* user = "USER";    
	char *printMach;
	char *printUser;
	char *printDir;
				  
	printMach = getenv(mach);
	printUser = getenv(user);
	printDir = getenv(workDir);			 

	printf("%s@%s: %s => ", printUser, printMach, printDir);

}


void ioCmd(char** argv)
{
	int ID;
	int now;

	int numline = 0;

	char proc[256] = "/proc/";
	char thepid[256];
	char place[256] = "/io";
	char ioStr[256];

	pid_t childPID; 
	childPID = fork();

	switch(checkZero(childPID)){
	case 2:
	{
		execv(argv[0], argv);
		printf("Not able to execute: \n");
		DisplayArgs(argv);
		exit(1);
		break;
	}
	case 1:
	{
		waitpid(childPID, &now, 0);
		ID = getpid();
		sprintf(thepid, "%i", ID);
		strcat(proc, thepid);
		strcat(proc, place);
		FILE* ioFile;
		ioFile = fopen(proc, "r");

		while(fgets(ioStr, sizeof(ioStr), ioFile))
		{
			if ((numline == 3) || (numline == 7) ||
			    (numline == 8) || (numline == 12))
			    {
				printf("%s", ioStr);
			    }
			numline++;
		}
		fclose(ioFile);
		break;
	}
	case 0:
	{
		printf("Fork has failed\n");
		exit(1);
		break;
	}
	}
}

void etimeCmd(char** argv)
{
	int now;
	struct timeval timeofday;
	gettimeofday(&timeofday, NULL);
	double beginning_time=timeofday.tv_sec+(timeofday.tv_usec/1000000.0);

	pid_t childPID = fork();

	switch(checkZero(childPID)){
	  case 2:
	  {
		execv(argv[0], argv);
                printf("Not able to execute: \n");
                DisplayArgs(argv);
                exit(1);
		break;
	  }
	  case 1:
		waitpid(childPID, &now, 0);
		break;
	  case 0:
		printf("Fork has failed\n");
		break;
	}


	gettimeofday(&timeofday, NULL);
	double finishing_time=timeofday.tv_sec+(timeofday.tv_usec/1000000.0);
	printf("Elapsed Time: %f\n", finishing_time-beginning_time);
}

int checkZero(int tocheck)
{
	if (tocheck == 0)
		return 2;
	else if (tocheck > 0)
		return 1;
	else
		return 0;
}

void KillZombies() {

    pid_t pid;
    while ( ( pid = waitpid( -1, 0, WNOHANG ) )  )
        
        if ( pid == -1 || pid == 0 )
            break;

}
