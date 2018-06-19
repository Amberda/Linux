#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void* thread_run(void* arg)
{
    //pthread_detach(pthread_self());
    printf("%s\n",(char*)arg);
    return NULL;
}

int main()
{
    pthread_t tid;
    pthread_create(&tid,NULL,thread_run,"thread is run...");

    int ret;
    sleep(1);
    ret=pthread_join(tid,NULL);
    if(ret==0)
    {
        printf("wait success\n");
    }
    else
    {
        printf("wait failed\n");
    }
    return 0;
}
