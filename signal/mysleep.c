#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

void handler(int signo)
{

}

int mysleep(int second)
{
    struct sigaction set,oset;
    int s=0;
    set.sa_handler=handler;
    sigemptyset(&set.sa_mask);
    set.sa_flags=0;
    sigaction(SIGALRM,&set,&oset);
    alarm(second);
    pause();
    s=alarm(0);
    sigaction(SIGALRM,&oset,NULL);
    return s;
}

int main()
{
    while(1)
    {
        printf("haha\n");
        mysleep(1);
    }
    return 0;
}
