#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

int main()
{
    if(fork()==0)
    {
        sleep(3);
        printf("child return pid = %d\n",getpid());
        exit(1);
    }
    else
    {
        int i;
        if(i=(waitpid(-1,NULL,0))==-1)
        {
            perror("waitpid");
            exit(0);
        }
        printf("wait child\n");
        while(1)
        {
            sleep(1);   
        }
    }
}
