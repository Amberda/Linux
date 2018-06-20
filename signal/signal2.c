#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

void handler(int s)
{
    printf("recv %d sig\n",s);
}

int main()
{
    signal(SIGUSR1,handler);
    kill(getpid(),SIGUSR1);
}
