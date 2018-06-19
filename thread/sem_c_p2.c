#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <time.h>
#include <pthread.h>

#define M 10
#define C 3
#define P 3
int ring[M];//环形队列，利用数组和%运算实现

sem_t sem_data;//数据量
sem_t sem_blank;//空格子量

pthread_mutex_t lock1=PTHREAD_MUTEX_INITIALIZER;//消费者之间的互斥
pthread_mutex_t lock2=PTHREAD_MUTEX_INITIALIZER;//生产者之间的互斥

void* runC(void* arg)
{
    static int i=0;
    int d;
    while(1)
    {
        pthread_mutex_lock(&lock1);//加锁，必须给所有可能互斥的代码加
        sem_wait(&sem_data);
        d=ring[i];
        printf("consumer data:%d\n",d);
        i++;
        i%=M;
        sem_post(&sem_blank);
        pthread_mutex_unlock(&lock1);
        sleep(3);
    }
}

void* runP(void* arg)
{
    int data=0;
    static int i=0;
    while(1)
    {
        pthread_mutex_lock(&lock2);
        data=rand()%100+1;
        sem_wait(&sem_blank);
        ring[i]=data;
        printf("product data:%d\n",data);
        i++;
        i%=M;
        sem_post(&sem_data);
        pthread_mutex_unlock(&lock2);
        sleep(2);
    }
}

int main()
{
    srand((unsigned long)time(NULL));

    pthread_t consumer[C];
    pthread_t product[P];
    int i=0;
    for(i=0;i<C;i++)
    {
        pthread_create(&consumer[i],NULL,runC,NULL);
    }
    for(i=0;i<P;i++)
    {
        pthread_create(&product[i],NULL,runP,NULL);
    }
    sem_init(&sem_data,0,0);
    sem_init(&sem_blank,0,M);

    for(i=0;i<C;i++)
    {
        pthread_join(consumer[i],NULL);
    }
    for(i=0;i<P;i++)
    {
        pthread_join(product[i],NULL);
    }

    sem_destroy(&sem_data);
    sem_destroy(&sem_blank);

    pthread_mutex_destroy(&lock1);
    pthread_mutex_destroy(&lock2);

    return 0;
}
