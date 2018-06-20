#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>

void printpend(sigset_t *set)
{
    int i=0;
    for(;i<=31;++i)
    {
        
    }
}

int main()
{
    sigset_t* set,oset;
    sigemptyset(&set);
    sigaddset(&set,SIGINT);
    sigprocmask(SIG_BLOCK,&set,&oset);
    while(1)
    {
        sigpending(&set);
        printpend(&set);
        sleep(1);
    }
    return 0;
}
