#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>


int mysystem(const char* cmd)
{
    pid_t pid;
    int status;
    if(cmd==NULL)
    {
        return 1;
    }
    pid=fork();
    if(pid==-1)
    {
        return -1;
    }
    if(pid==0)
    {//child
        execl("/bin/sh","sh","-c",cmd,NULL);
        exit(127);
    }
    else
    {//parent
        while(waitpid(pid,&status,0)==-1)
        {
            if(errno==EINTR)
            {
                continue;
            }
            status = -1;
            break;
        }
    }
    return status;
}

int main()
{
    mysystem("ls -l | wc -l");
}
