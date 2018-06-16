#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>



int main()
{
    pid_t pid;
    if((pid=fork())==-1)
    {
        perror("fork");
        exit(1);
    }
    if(pid==0)
    {
        int i=0;
        while(1)
        {
            printf("$$\n");
            sleep(1);
          //  if(++i == 3)
          //      break;
        }
        exit(0);
    }
    else
    {
        int st;
        pid_t r;
        r=wait(&st);
        if(r==-1)
        {
            perror("wait");
        }
        printf("pid=%d, r=%d, ret=%d \n",pid,r,(st>>8)&0xFF);
        if(WIFEXITED(st))
        {
            printf("exitcode = %d\n",WEXITSTATUS(st));       
        }
        else if(WIFSIGNALED(st))
        {
            printf("signal code = %d\n",WTERMSIG(st));
        }
    }
}
