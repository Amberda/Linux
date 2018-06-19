#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int ticket=500;

//pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex;

void* sell_tic(void *arg)
{
    while(1)
    {   
        pthread_mutex_lock(&mutex);
        if(ticket>0)
        {
            usleep(1000);
            printf("%s get ticket%d\n",(char*)arg,ticket);
            ticket--;
            pthread_mutex_unlock(&mutex);
        }
        else
        {
            pthread_mutex_unlock(&mutex);
            break;   
        }
    }
}

int main()
{
    pthread_mutex_init(&mutex,NULL);
    pthread_t tid1,tid2,tid3,tid4;
    pthread_create(&tid1,NULL,sell_tic,"thread 1");  
    pthread_create(&tid2,NULL,sell_tic,"thread 2");  
    pthread_create(&tid3,NULL,sell_tic,"thread 3");  
    pthread_create(&tid4,NULL,sell_tic,"thread 4");  

    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    pthread_join(tid3,NULL);
    pthread_join(tid4,NULL);
    
    pthread_mutex_destroy(&mutex);
    return 0;
}
