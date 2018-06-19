#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <time.h>
#include <pthread.h>

#define M 10
int ring[M];//环形队列，利用数组和%运算实现

sem_t sem_data;//数据量
sem_t sem_blank;//空格子量

void* runC(void* arg)
{
    int i=0;
    int d=0;
    while(1)
    {
        sem_wait(&sem_data);
        d=ring[i];
        sem_post(&sem_blank);
        printf("consumer data:%d\n",d);
        i++;
        i%=M;
        sleep(1);
    }
}

void* runP(void* arg)
{
    int data=0;
    int i=0;
    while(1)
    {
        data=rand()%100+1;
        sem_wait(&sem_blank);
        ring[i]=data;
        sem_post(&sem_data);
        printf("product data:%d\n",data);
        i++;
        i%=M;
        sleep(2);
    }
}

int main()
{
    srand((unsigned long)time(NULL));
    sem_init(&sem_data,0,0);
    sem_init(&sem_blank,0,M);

    pthread_t c,p;
    pthread_create(&c,NULL,runC,NULL);
    pthread_create(&p,NULL,runP,NULL);

    pthread_join(c,NULL);
    pthread_join(p,NULL);

    sem_destroy(&sem_data);
    sem_destroy(&sem_blank);

    return 0;
}
