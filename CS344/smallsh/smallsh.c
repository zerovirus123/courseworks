//
//  Filename: smallsh.c
//  Description: Small shell program for the Unix/Linux operating, with a similar but scaled down functionality
//  compared to that of a Bourne shell.
//  The main 3 features are cd, exit and status, with other auxillary functionalities.
//  Support comments, beginning with the # character
//
//  Created by Brian Sia on 7/26/16.


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

//the required arguments and characters the shell can accept, as dictated by assignment
#define CHAR_MAX 2048
#define ARGS_MAX 512
#define TOK_DELIM " \t\r\n\a"	// for strtok delimiters

//structure that holds the command flags and strings needed for shell functionalities
//namely, name of input file, name of output file, status flags for i/o redirection
//argc counter and an int to determine if a process runs in background
//for background and i/o redirection, 1 for yes, 0 for no
struct shellStatus {

    char *inputFile;
    char *outputFile;
    int redirInput;
    int redirOutput;
    int argc;
    int background;
    
};

//for readability purposes
typedef enum {
    
    STOP,
    CONTINUE,
    INPUT,
    OUTPUT
    
} readStatus;

int main(int argc, char**argv);
char *readLine();
char **splitLine(char *line, struct shellStatus *cmd);
void launchCmd(char **args, struct shellStatus *cmd);
void fgProcess(pid_t pid, char *exitStatus);
void killZombie();

//main()
//shell starts executing here
//a shell is basically an infinite while loop that runs while the exit flag is false
//in this method, flags and signal handlers are initialized
int main(int argc, char **argv){

    int index, exitFlag=0, processCount = 0;
    pid_t process[100];
    char exitStatus[CHAR_MAX] = "No previous foreground process.\n";

    //initialize signal event handlers
    struct sigaction defaultAct, ignoreAct;
    defaultAct.sa_handler = SIG_DFL;
    ignoreAct.sa_handler = SIG_IGN;
    
    //signal interrupt is ignored in the main shell loop
    //The NULL means that previous actions/processes is not considered
    sigaction(SIGINT, &ignoreAct, NULL);
    
    //main subroutine calls and logic of the small shell runs here
    do{
        char *line = NULL;
        char **args;
        
        struct shellStatus *cmdFlags = malloc(sizeof(struct shellStatus));
        
        cmdFlags->redirInput = 0;
        cmdFlags->redirOutput = 0;
        cmdFlags->argc = 0;
        cmdFlags->background = 0;
        
        killZombie();
        
        printf(": "); //prompt
        
        //parse user input
        line = readLine();
        args = splitLine(line, cmdFlags);
        
        //if a blank line is entered, set exit shell flag to 0,
        if(cmdFlags->argc ==0 || args[0] == NULL){
            exitFlag = 0;
        }
        
        //if user entered # for comments
        else if (strncmp(args[0], "#", 1) == 0) //strncmp can compare just one character
        {
            exitFlag = 0;
        }
        
        //exit command, one of the built-in functionality of the shell
        else if( strcmp(args[0], "exit") == 0 ){
            
            //if there are more than two argument
            if(cmdFlags->argc > 2){
                fprintf(stdout, "exit: unexpected arguments\n");
                fflush(stdout);
            }
            
            //if there is an unexpected argument
            else if (args[1]){
                fprintf(stdout, "exit: unexpected arguments '%s'\n", args[1]);
                fflush(stdout);
            }
            
            else{ //just exit
                exitFlag = 1; //set the flag to exit the while loop
                fprintf(stdout, "killing background processes\n");
                fflush(stdout);
                
                //kill all the processes
                for(index = 0; index < processCount; index++){
                    kill(process[processCount], SIGTERM);
                }
            }
        }
        
        //cd command, one of the built-in functionality of the shell
        else if( strcmp(args[0], "cd") == 0 ){
        
            //if there are more than two argument
            if(cmdFlags->argc > 2){
                fprintf(stdout, "exit: unexpected arguments\n");
                fflush(stdout);
            }
            
            //if there is an unexpected argument
            else if (args[1]){
                fprintf(stdout, "exit: unexpected arguments '%s'\n", args[1]);
                fflush(stdout);
            }

            else{
                chdir(getenv("home"));
            }
        }
        
        //status command, one of the built-in command of the shell
        else if ( strcmp(args[0], "status") == 0 )
        {
            fprintf(stdout, "%d\n", exitFlag);
            fflush(stdout);
        }
        
        //otherwise, just execute the typed in command
        else{
            
            pid_t cpid; //creator PID of type pid_t (pid_t is platform portable)
            cpid = fork(); //creates a child process by duplicating the current process
            //child process gets its own unique PID
            
            if (cpid == 0) //if child PID == 0
            {
                if (!cmdFlags->background)	// if it is a foreground process
                {
                    // allow interruption of foreground processes with signals
                    sigaction(SIGINT, &defaultAct, NULL);
                }
                launchCmd(args, cmdFlags);
            }
            
            else if (cpid == -1)//fork() fails to run
            {
                fprintf(stderr, "fork call failed\n");
                exit(EXIT_FAILURE);
            }
            
            else
            {
                if (cmdFlags->background)
                {
                    process[processCount] = cpid;
                    processCount++;
                    printf("background pid: %d\n", cpid);
                    fflush(stdout);
                }
                
                else
                {
                    fgProcess(cpid, exitStatus);
                }
            }
            
        }
        
        // deallocate memory
        free(line);
        free(args);
        free(cmdFlags);
        
    }while(!exitFlag);
    return 0;

}

//readLine()
//reads in stdin input with getline, then returns the line
//preconditions:
//postconditions:
char *readLine(){
    
    char *line = NULL;
    size_t buff = CHAR_MAX; //not using ssize_t because that is a POSIX extension, and for absolute portability
    getline(&line, &buff, stdin);
    return line;
    
}

//killZombie()
//removes zombie processes from the process table
//although zombie process consumes little memory, they take up finite PID slots
//accumulating zombies will leave little PIDs for other processes
//preconditions:
//postconditions: bg process complete status written to stdout
void killZombie(){

    pid_t cpid;
    int status;
    
    cpid = waitpid(-1, &status, WNOHANG);
    
    if (cpid > 0) //wait for child whose PID is equal to -1
    {
        if (WIFEXITED(status)) //true if child process is terminated normally
            //by calling exit or returning from main()
        {
            printf("bg process %d is done: exit value %d\n", cpid, WEXITSTATUS(status));
            fflush(stdout);
        }
        else if (WIFSIGNALED(status)) //true if child process is terminated by a signal
        {
            printf("bg process %d is done: terminated by signal %d\n", cpid, WTERMSIG(status));
            fflush(stdout);
        }
    }

}

//splitLine()
//takes user input line and shellStatus struct, then parse input, setting the flags in the struct
//Method determines if user wants I/O redirection, I/O files, or background processes
//returns the parsed command as an array of strings
//preconditions: receives user input as string, and a shellStatus struct
//postconditions: command flags in struct is set, returns parsed command as a array of strings
char **splitLine(char *line, struct shellStatus *cmd){


    int buffSize = ARGS_MAX;
    //readStatus: 0 = stop, 1 = continue, 2 = input, 3 = output
    readStatus readStat = CONTINUE;
    
    //token(s) are used to handle the command arguments
    char **tokens = malloc(buffSize * sizeof(char*));
    char *token;
    
    cmd->redirInput = 0;
    cmd->redirOutput = 0;
    cmd->argc = 0;
    cmd->background = 0;
    
    if(!tokens || !token){
        fprintf(stderr, "smallsh allocation error\n");
        exit(EXIT_FAILURE); //portable version of exit(1)
    }
    
    //break the input string into meaningful elements called tokens, as governed by the delimiter
    token = strtok(line, TOK_DELIM);
    
    while( token != NULL && readStat != STOP ){
        
        //running bg process
        if (strcmp(token, "&") == 0)
        {
            cmd->background = 1;
            readStat = STOP;
        }
        
        //input redir
        else if (strcmp(token, "<") == 0)
        {
            cmd->redirInput = 1;
            readStat = INPUT;
        }
        
        //output redirection indicated
        else if (strcmp(token, ">") == 0)
        {
            cmd->redirOutput = 1;
            readStat = OUTPUT;
        }
        
        else //token is not a symbol, but a filename or cmd argument
        {
            if (readStat == INPUT)
            {
                cmd->inputFile = token;
            }
            else if (readStat == OUTPUT)
            {
                cmd->outputFile = token;
            }
            else
            {
                tokens[cmd->argc] = token;
                cmd->argc++;
                tokens[cmd->argc] = NULL;
            }
        }
        
        token = strtok(NULL, TOK_DELIM); //continue searching for other tokens marked by delimiter
    
    }
    
    return tokens;
}

//launchCmd()
//executes a provided command line, with additional information from shellStatus struct
//preconditions: appropriate flags set in shellStatus. takes in an array of strings as commands, and the shellStatus struct
//postconditions: desired command is executed
void launchCmd(char **args, struct shellStatus *cmd){

    int inputFD, outputFD; //file descriptors
    char fileName[CHAR_MAX] = "";
    
    if(cmd->redirInput == 1){
        
        inputFD = open(cmd->inputFile, O_RDONLY);
        
        if (inputFD == -1) //error opening
        {
            fprintf(stderr, "smallsh: cannot open %s for input\n", cmd->inputFile);
            exit(EXIT_FAILURE);
        }
        
        //duplicates inputFD, so fd with number 0 points to the same input file
        //the two fd may be used intercangeably, as they have the same file offset and file status flags
        //however, they have different file descriptor flags
        if (dup2(inputFD, 0) == -1)
        {
            fprintf(stderr, "smallsh: cannot open %s for input\n",
                    cmd->inputFile);
            exit(EXIT_FAILURE);
        }
    
    }
    
    else if(cmd->background){ //no specified file
    
        inputFD = open("/dev/null", O_RDONLY);
        
        //anything written to dev/null will just be discarded
        //essentially, this discards any output from the command being executed
        if (inputFD == -1){
            fprintf(stderr, "smallsh: cannot open /dev/null for input\n");
            exit(EXIT_FAILURE);
        }
        
        if (dup2(inputFD, 0) == -1){
            fprintf(stderr, "smallsh: cannot open /dev/null for input\n");
            exit(EXIT_FAILURE);
        }
    }
    
    // redirect output
    if (cmd->redirOutput)
    {
        // open new output file to redirect
        outputFD = open(cmd->outputFile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        //opens the file based on path, setting the flag to write only, create the file if it does not exist,
        //and if file exists, truncate it to 0 once it is opened.
        //0644 is equivalent to r+w for user, read for group and others respectively
        
        if (outputFD == -1)
        {
            fprintf(stderr, "smallsh: cannot open %s for output\n", cmd->outputFile);
            exit(EXIT_FAILURE);
        }
        
        // redirect stdout so fd 1 points to outputfd
        if (dup2(outputFD, 1) == -1)
        {
            fprintf(stderr, "smallsh: cannot open %s for output\n", cmd->outputFile);
            exit(EXIT_FAILURE);
        }
    }
    
    //replace current process image with the new process image
    execvp(args[0], args);
    
    // if there are no valid entry commands
    fprintf(stderr, "%s: no such file or directory\n", args[0]);
    exit(EXIT_FAILURE);

}

//fgProcess()
//handles the statuses of foreground processes, especially the exit status
//preconditions:
//postconditions: exit status is modified based on process exit status
void fgProcess(pid_t pid, char *exitStatus){
    
    int status;
    
    waitpid(pid, &status, 0);
    
    if (WIFEXITED(status)) //if fg process ended normally
    {
        sprintf(exitStatus, "exit value:  %d", WEXITSTATUS(status));
    }
    if (WIFSIGNALED(status)) //if fg process terminated by a signal
    {
        sprintf(exitStatus, "terminated by signal %d", WTERMSIG(status));
        printf("%s\n", exitStatus);
        fflush(stdout);
    }

}


