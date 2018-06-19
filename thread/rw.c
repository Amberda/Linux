#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int count;
pthread_rwlock_t rwlock;

void* writeRun(void* arg)
{
    int i = *(int*)arg;
    printf("%d\n",i);
    int t;

    while(1)
    {
        t=count;
        pthread_rwlock_wrlock(&rwlock);
        printf("write id (%d), id:%x, count=%d,++count=%d\n",i,pthread_self(),t,++count);
        pthread_rwlock_unlock(&rwlock);
        usleep(50000);
    }
}

void* readRun(void* arg)
{
    int i = *(int*)arg;

    while(1)
    {
        pthread_rwlock_rdlock(&rwlock);
        printf("read id (%d), id:%x, count=%d\n",i,pthread_self(),count);
        pthread_rwlock_unlock(&rwlock);
        usleep(5000);
    }
}

int main()
{
    pthread_t tid[8];

    pthread_rwlock_init(&rwlock,NULL);

    int i=0;
    for(i=0;i<3;i++)
    {
        pthread_create(&tid[i],NULL,writeRun,i);
    }
    for(i=3;i<8;i++)
    {
        pthread_create(&tid[i],NULL,readRun,i);   
    }
    for(i=0;i<8;i++)
    {
        pthread_join(tid[i],NULL);
    }
    pthread_rwlock_destroy(&rwlock);
    return 0;
}
