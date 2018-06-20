#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>

void handler(int signo)
{
    printf("%d,you can't kill me\n",signo);
}

int main()
{
    __sighandler_t hand=NULL;
    if((hand=signal(SIGINT,handler))==SIG_ERR)
    {
        perror("signal");
        return -1;
    }
    while(1);
    return 0;
}
