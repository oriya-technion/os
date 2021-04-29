
//		commands.cpp
//********************************************
using namespace std;
#include "commands.h"

vector<string> history;

//********************************************
// function name: ExeCmd
// Description: interperts and executes built-in commands
// Parameters: pointer to jobs, command string
// Returns: 0 - success,1 - failure
//**************************************************************************************
int ExeCmd(vector<Job> jobs, char* lineSize, char* cmdString)
{
	char* cmd;
	char* args[MAX_ARG];
	char pwd[MAX_LINE_SIZE];
	char* delimiters = " \t\n";
	int i = 0, num_arg = 0;
	bool illegal_cmd = false; // illegal command
    cmd = strtok(lineSize, delimiters);
	if (cmd == NULL)
		return 0;
   	args[0] = cmd;
	for (i=1; i<MAX_ARG; i++)
	{
		args[i] = strtok(NULL, delimiters);
		if (args[i] != NULL)
			num_arg++;

	}

	if (strcmp(cmd, "history"))
	{
        string str(cmdString);
        if (history.size() + 1 > MAX_HISTORY_SIZE)
            history.erase(history.begin());
        history.push_back(str);
    }
/*************************************************/
// Built in Commands PLEASE NOTE NOT ALL REQUIRED
// ARE IN THIS CHAIN OF IF COMMANDS. PLEASE ADD
// MORE IF STATEMENTS AS REQUIRED
/*************************************************/
	if (!strcmp(cmd, "cd") )
	{
        if (!strcmp(args[1], "-") )
        {
            if (!chdir("..")){
                char* path = get_current_dir_name();
                printf("%s\n", path);
            }
            else {
                printf("smash error: > %s %s\n", args[1], strerror(errno));
                return 1;
            }
        }
        else
        {
        if (chdir(args[1]))
            printf("smash error: > %s %s\n", args[1], strerror(errno));
            return 1;
        }
	}
	/*************************************************/
	else if (!strcmp(cmd, "pwd"))
	{
        char* path = get_current_dir_name();
        printf("%s\n", path);
	}
	/*************************************************/
	else if (!strcmp(cmd, "history"))
	{
        for (vector<string>::const_iterator i = history.begin(); i != history.end(); ++i)
            cout << *i << endl;
	}
	/*************************************************/
	else if (!strcmp(cmd, "jobs"))
	{
        for(int i=0; i<jobs.size(); i++){
            string stopped = (jobs[i].stopped()) ? "(stopped)" : "";
            cout << "[" << i << "] " << jobs[i].cmd() << " : " << jobs[i].pid() << " " << jobs[i].get_time() << " secs " << stopped << endl;;
        }
    }
    /*************************************************/
    	else if (!strcmp(cmd, "kill"))
	{
        if (num_arg < 2){
            printf("smash error: > kill %s ‫–‬ ‫‪job‬‬ ‫‪does‬‬ ‫‪not‬‬ ‫‪exist‬‬\n", args[1]);
            return 1;
        }
        else {
        args[2]++;
       // if (!kill(jobs(args[2]), args[1]))
       //     printf("smash error: > %s %s\n", args[1], strerror(errno));
        }
	}
	/*************************************************/
	else if (!strcmp(cmd, "showpid"))
	{
       pid_t p = (long)getpid();
       printf("smash pid is %ld\n", p);
	}
	/*************************************************/
	else if (!strcmp(cmd, "fg"))
	{

	}
	/*************************************************/
	else if (!strcmp(cmd, "bg"))
	{

	}
	/*************************************************/
	else if (!strcmp(cmd, "quit"))
	{
        if (num_arg < 2) exit(0);
        pid_t p = getpid();
        kill(p, SIGTERM);
	}
    /*************************************************/
	else if (!strcmp(cmd, "cp"))
	{
        struct stat statbuf;

        int f1 = open(args[1], O_RDONLY);
        if (f1 == -1){
            printf("smash error: > %s %s\n", args[1], strerror(errno));
            return 1;
        }

        if(fstat(f1, &statbuf)==-1){
            printf("smash error: > %s %s\n", args[1], strerror(errno));
            return 1;
        }

        int f2 = open(args[2], O_RDWR|O_CREAT|O_TRUNC|S_IWUSR|S_IRUSR);


        ssize_t cpoied = sendfile(f2, f1, NULL, statbuf.st_size);
        if(cpoied==-1){
            printf("smash error: > %s %s\n", args[1], strerror(errno));
            return 1;
        }
        else
            printf("%s has been copied to %s\n", args[1], args[2]);
        close(f1);
        close(f2);
	}
    /*************************************************/
	else if (!strcmp(cmd, "diff"))
	{
        struct stat statbuf1;
        struct stat statbuf2;

        int f1 = open(args[1], O_RDONLY);
        if (f1 == -1){
            printf("smash error: > %s %s\n", args[1], strerror(errno));
            return 1;
        }
        if(stat(args[1], &statbuf1)==-1){
            printf("smash error: > %s %s\n", args[1], strerror(errno));
            return 1;
        }

        int f2 = open(args[2], O_RDONLY);
        if (f2 == -1){
            printf("smash error: > %s %s\n", args[2], strerror(errno));
            return 1;
        }
        if(fstat(f2, &statbuf2)==-1){
            printf("smash error: > %s %s\n", args[2], strerror(errno));
            return 1;
        }

        if (statbuf1.st_size != statbuf2.st_size) {
            printf("1\n");
            return 0;
        }

        void* buf1 = (void*) malloc(statbuf1.st_size);
        void* buf2 = (void*) malloc(statbuf2.st_size);

        if (buf1==NULL || buf2==NULL) {
            printf("smash error: > %s cannot allocate memmory\n", args[1]);
            return 1;
        }

        if(read(f1,buf1,statbuf1.st_size)==-1){
            printf("smash error: > %s %s\n", args[1], strerror(errno));
            return 1;
        }
        if(read(f2,buf2,statbuf2.st_size)==-1){
            printf("smash error: > %s %s\n", args[2], strerror(errno));
            return 1;
        }

        int same = memcmp(buf1,buf2,statbuf1.st_size);
        same = (same!=0) ? 1 : 0;
        printf("%d\n", same);
        close(f1);
        close(f2);
	}
	else // external command
	{
 		ExeExternal(args, cmdString);
	 	return 0;
	}
	if (illegal_cmd == true)
	{
		printf("smash error: > \"%s\"\n", cmdString);
		return 1;
	}
    return 0;
}
//**************************************************************************************
// function name: ExeExternal
// Description: executes external command
// Parameters: external command arguments, external command string
// Returns: void
//**************************************************************************************
void ExeExternal(char *args[MAX_ARG], char* cmdString)
{
	int pID;
    	switch(pID = fork())
	{
    		case -1:
					// Add your code here (error)

					/*
					your code
					*/
        	case 0 :
                	// Child Process
               		setpgrp();

			        // Add your code here (execute an external command)

					/*
					your code
					*/

			default:
                	// Add your code here


					int x = 5;

	}
}
//**************************************************************************************
// function name: ExeComp
// Description: executes complicated command
// Parameters: command string
// Returns: 0- if complicated -1- if not
//**************************************************************************************
int ExeComp(char* lineSize)
{
	char ExtCmd[MAX_LINE_SIZE+2];
	char *args[MAX_ARG];
    if ((strstr(lineSize, "|")) || (strstr(lineSize, "<")) || (strstr(lineSize, ">")) || (strstr(lineSize, "*")) || (strstr(lineSize, "?")) || (strstr(lineSize, ">>")) || (strstr(lineSize, "|&")))
    {
		// Add your code here (execute a complicated command)

		/*
		your code
		*/
	}
	return -1;
}
//**************************************************************************************
// function name: BgCmd
// Description: if command is in background, insert the command to jobs
// Parameters: command string, pointer to jobs
// Returns: 0- BG command -1- if not
//**************************************************************************************
int BgCmd(char* lineSize, vector<Job> * jobs)
{

	char* Command;
	char* delimiters = " \t\n";
	char *args[MAX_ARG];
	int pID=1234;
	if (lineSize[strlen(lineSize)-2] == '&')
	{
		lineSize[strlen(lineSize)-2] = '\0';
        string cmd(lineSize);

		// Add your code here (execute a in the background)
		time_t creationTime = time(NULL);
        Job* newJob = new Job(cmd, creationTime, pID);
        jobs->push_back(*newJob);
	}
	return -1;
}
