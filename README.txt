COP4610 - Project 1
- the purpose of this project was to familiarize us with the mechanics of process control through the implementation of a shell user interface. Including child and parent processes, the steps needed to create and manipulate a new process, and introduction to user-input parsing and verification. We were able to come to understand how to input/output redirection, pipes, and background processes. 

Problem statement: 
- we were to design and implement a basic shell interface that supports i/o redirection, pipes, background processing and a series of built in functions. 
Solution: our project is robust (ex. it does not crash). And all of the required features adhere to the operational semantics of the basic shell. 

Assumptions that we made: 
- first and foremost this program is assumed to only be able to run on Linprog
- we worked through the could not's of the program as we began. Establishing what inputs should be taken in for the manipulation of the shell for instance we assumed that certain characters, global variables, and quotations could not be entered as input. 
- we also wrote the background process code so that a stirng that we wanted, would print instead of what the user types into the shell. 


Project Team Members
- Holly Culver
- Abby Perry
- Hunter Davis

Process of Creation: 
  We began the project by divided it into what we thought were the most complicated sections: Holly = background, Abby = piping, Hunter = Parsing and we planned to meet up again and work on functionality. We came accross some issues as we worked such as variable naming, type declarations, and how to run the program without individual testing completed. 
  We decided to continue the remainder of the project together as we would be able to discuss as we went. This was much more successful, even though we had to go back and change a few things that we worked on as individuals becasue fo the initial lack of communication we were eventually able to work out the glitches. 
    Next, we met up to create  MyFunctions and MyShell libraries that consisted of our running code. We were able to test all the work we had previously done as well as follow the project decription to create our functions and tests all appropriate project requirements.
    Once the complicated functions were successfully completed, we began testing all shell commands and our biggest issue came with etime and limits. We had a constant SegFault and we could not figure out where it was taking place
    Plan of actions: we ran a gdp core dump function to find where the seg fault was taking place. CoreDump told us this was a SegFault 11 error and we could only assume that this was a sizing issue and somewhere we were running out of space for our function. So we started fromt the bottom, up. Retracing all of our steps we wrote a number of functions until the program was once again successful. 
  Several hours and a day late, we have run our program successfully. We learned a lot throughout this process and we were able to work together as a team to overcome the issues we faced. 

Division of Labor: 
- Holly Culver: Background Processing, Function declarations, code testing, pipe testing, makefile declaration
- Abby Perry: Piping, shell, process testing, code testing
- Hunter Davis: Parsing, background processing, code testing

How to Compile:
- Using the Makefile, type 'make'. The executable will be named MyShell.x
- Type MyShell.x

How to clean: 
-make clean 

Tar File Contents:
README.txt
makefile
MyFunctions.c
MyFunctions.h
MyParse.c
MyParse.h
MyProcesses.c
MyProcesses.h
MyShell.c
MyShell.h
main.c


Completed using: linprog


Brief Description of Each File:

-MyParse: grabs input from the user and extracts the command name, the arguments, and background execution indicators. Allocates memory for the parsing arguments and converts them to dynamic character arrays. Passes information from one process to the next, allows for communicaiton between multiple function calls. Manages and calculates the number of commands separated by "|"

MyProcesses: handles, creates, and checks the created processes to be manipulated for the completion of the shell 

-MyFunctions: Allocates and deallocates memory, checks in files exist, makes/deletes directories, creates/ changes/ builds paths, and prints arguments to screen.  Creates a background to test commands given from the user as input. Completes functions such as I/O redirection and executes the pipe function and does error checking for both of these. 

-MyShell: single run shell command that initiates the running of the shell process 

Known Bugs:

-For background processing there is supposed to be an error when typing CMD1 < & FILE but the shell tries to execute this command

-When the io command is executed the ouput is different from the ouput expected in the project description

Unfinished Portions:
