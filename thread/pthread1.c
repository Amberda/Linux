#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/syscall.h>


void* thread_run(void* arg)
{
    while(1)
    {   
        printf("thread is run...,tid=%d\n",pthread_self());
        sleep(1);
    }
}

int main()
{
    pthread_t tid;
    int ret = pthread_create(&tid,NULL,thread_run,NULL);

    if(ret!=0)
    {
        perror("pthread_create");
        return -1;
    }
    while(1)
    {
        printf("I am running\n");
        sleep(1);
    }
    return 0;
}
