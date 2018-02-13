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
		else if (strcmp(argv[0], "exit") == 0)
		{
			MemFunc(argv);
			printf("Exiting Shell...\n");
			KillZombies();
			exitQueue();
			exit(0);
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


		else if (strcmp(argv[0], "io") == 0)
		{
			if (GetSize(argv) > 1)
			{
				argv = RemoveArr(argv, 0);
				ioCmd(argv);
			}

		}
		else if (strcmp(argv[0], "etime") == 0)
		{
			if (GetSize(argv) > 1)
			{
				argv = RemoveArr(argv, 0);
				etimeCmd(argv);
			}
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
		printf("No such directory\n");
		break;
	}
	}

}



void theExtern(char** argv, int back, char* cmd)
{
	int status;
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
			
			waitpid(pid, &status, WNOHANG);
			
			handleQueue(newPro(pid, -1, cmd));
			
			
			break;
		}
		case -1:
		{
			
			waitpid(pid, &status, 0);
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
				
	// update background iterator
	back = StringCheck(argv, "&");
	char* cmd = Convert(argv);
	if (back != -1)
	{
		argv = RemoveArr(argv, back);
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
	if (back != -1)
	{
		argv = RemoveArr(argv, back);
	}
	handleIO(argv, 0, filename, back, cmd);
	free(filename);
	free(cmd);
	
	return argv;
}

char** externPipe(char** argv, int numpipe, int back)
{
	char* cmd = Convert(argv);
	if (back != -1)
	{
		argv = RemoveArr(argv, back);
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

void handleIO(char** argv, int dir, char* filename, int back, char* cmd)
{
	// output redirection
	switch(dir){
	case 0:
	{
		int status;
		int fd = open(filename, O_CREAT|O_WRONLY|O_TRUNC, 0777);
		if (fd == -1)
		{
			printf("Cannot open the file filename: %s\n", filename);
			exit(1);
		}
		pid_t pid = fork();

		switch(checkZero(pid)){
		case 2:
		{
			close(1);
			dup(fd);
			close(fd);
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
				waitpid(pid, &status, WNOHANG);
				handleQueue(newPro(pid, -1, cmd));
				break;
			}
			case -1:
			{
				waitpid(pid, &status, 0);
				break;
			}
			}
			close(fd);
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
		int status;
		int fd = open(filename, O_RDONLY);
		switch(fd){
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
			dup(fd);
			close(fd);
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
				waitpid(pid, &status, WNOHANG);
				handleQueue(newPro(pid, -1, cmd));
				break;
			}
			case -1:
			{
				waitpid(pid, &status, 0);
				break;
			}
			}
			close(fd);
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
	int P_loc = StringCheck(argv, "|");
	int B_loc = StringCheck(argv, "&");
	
	if (B_loc == -1)
	{
		return 0;
	}
	
	// if not < > or |, no error
	if ((inp == -1) &&
		(outp == -1) &&
		(P_loc == -1))
	{
		return 0;
	}
	else
	{
		if (inp != -1)
		{
			// & neighbors <
			if (inp - 1 == B_loc || inp + 1 == B_loc)
			{
				printf("Incorrect format for executing background processing\n");
				return 1;
			}
		}
		if (outp != -1)
		{
			// & neighbors >
			if (outp - 1 == B_loc || outp + 1 == B_loc)
			{
				printf("Incorrect format for executing background processing\n");
				return 1;
			}
		}
		if (P_loc != -1)
		{
			// & neighbors |
			if (P_loc - 1 == B_loc || P_loc + 1 == B_loc)
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
	const char* directory = "PWD";
	const char* machine = "MACHINE";
	const char* user = "USER";    
	char *machine_out;
	char *user_out;
	char *directory_out;
				  
	machine_out = getenv(machine);
	user_out = getenv(user);
	directory_out = getenv(directory);			 

	printf("%s@%s: %s => ", user_out, machine_out, directory_out);

}


void ioCmd(char** argv)
{
	int childID;
	int status;

	int line_count = 0;

	char file[256] = "/proc/";
	char PID[256];
	char location[256] = "/io";
	char limit_string[256];

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
		waitpid(childPID, &status, 0);
		childID = getpid();
		sprintf(PID, "%i", childID);
		strcat(file, PID);
		strcat(file, location);
		FILE* limit_file;
		limit_file = fopen(file, "r");

		while(fgets(limit_string, sizeof(limit_string), limit_file))
		{
			if ((line_count == 3) || (line_count == 7) ||
			    (line_count == 8) || (line_count == 12))
			    {
				printf("%s", limit_string);
			    }
			line_count++;
		}
		fclose(limit_file);
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


/*
void ioacct( pid_t child ) {

    char filename[100];
    char test[100];

    int tempBytes = -1;
    int readBytes = -1;
    int writeBytes = -1;
    pid_t child_finished = -999;            // pid is never going to be this

    sprintf(filename, "/proc/%d/io", (int)child);
    FILE* proc_file = fopen(filename, "r"); // open the proc file for eading

    while ((child != child_finished)) {

        //read from the file
        int count = 0;
        while (fscanf(proc_file, "%s %d", test, &tempBytes) > 0) {
            if (count == 4) {
                readBytes = tempBytes;
            }
            if (count == 5) {
                writeBytes = tempBytes;
            }
            ++count;
        }
        child_finished = waitpid(-1, (int *)NULL, 0);
    }
    printf("\nBytes Read: %d\nBytes Written: %d\n", readBytes, writeBytes);
}

*/


void etimeCmd(char** argv)
{
	int status;
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
		waitpid(childPID, &status, 0);
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

    // Kill processes as long as we keep finding them
    while ( ( pid = waitpid( -1, 0, WNOHANG ) )  )
        
        // No zombie processes are found
        if ( pid == -1 || pid == 0 )
            break;

}
