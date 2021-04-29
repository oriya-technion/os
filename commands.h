#ifndef _COMMANDS_H
#define _COMMANDS_H
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <fcntl.h>
#include <errno.h>
#include <iostream>
#include <vector>
#define _GNU_SOURCE
#define MAX_LINE_SIZE 80
#define MAX_ARG 20
#define MAX_HISTORY_SIZE 50
extern std::vector<std::string> history;
class Job {
public:
    Job(std::string command, time_t startingTime, pid_t pid): _startingTime(startingTime), _stopped(false), _pid(pid){
        _cmd = command;
    };
    const int get_time(){
        time_t currentTime = time(NULL);
        return (currentTime - _startingTime);
    }

    const long pid(){return (long)_pid;}
    const std::string cmd(){return _cmd;}
    const bool stopped(){return _stopped;}
    void setStopped(const bool value) {_stopped = value;}

private:
    pid_t _pid;
    std::string _cmd;
    time_t _startingTime;
    bool _stopped;

};
int ExeComp(char* lineSize);
int BgCmd(char* lineSize, std::vector<Job> * jobs);
int ExeCmd(std::vector<Job> jobs, char* lineSize, char* cmdString);
void ExeExternal(char *args[MAX_ARG], char* cmdString);
#endif
