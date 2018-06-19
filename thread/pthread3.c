#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void* thread_run(void* arg)
{
    pthread_detach(pthread_self());
    printf("thread running...\n");
    sleep(1);
    return NULL;
}


int main()
{
    pthread_t t1;
    pthread_create(&t1,NULL,thread_run,NULL);
    sleep(1);

    int ret = pthread_join(t1,NULL);
    if(ret!=0)
    {
        printf("wait failed\n");
    }
    else
    {
        printf("wait sucess\n");
    }
    return 0;

}
