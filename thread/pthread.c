#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/syscall.h>


void* thread_run(void* arg)
{
    pid_t tid1;
    tid1=syscall(SYS_gettid);
    printf("tid = %d\n",tid1);
    int count=1;
    while(1)
    {
        printf("thread1 is running\n");
        sleep(1);
        count++;
        if(count==5)
            break;
    }
    return NULL;
}
void* thread2_run(void* arg)
{
    int count=1;
    while(1)
    {
        printf("thread2 is running\n");
        sleep(1);
        if(count++==5)
            break;
    }
    //return NULL;
}
void* thread3_run(void* arg)
{
    int count=1;
    while(1)
    {
        printf("thread3 is running\n");
        sleep(1);
        if(count++==5)
            break;
    }
    //return NULL;
}
void* thread4_run(void* arg)
{
    int count=1;
    while(1)
    {
        printf("thread4 is running\n");
        sleep(1);
        if(count++==5)
            break;
    }
    //return NULL;
    //pthread_exit(NULL);
    //pthread_cancel(pthread_self());
}

int main()
{
    pthread_t tid,tid2,tid3,tid4;
    int ret=pthread_create(&tid,NULL,thread_run,NULL);
    pthread_create(&tid2,NULL,thread2_run,NULL);
    pthread_create(&tid3,NULL,thread3_run,NULL);
    pthread_create(&tid4,NULL,thread4_run,NULL);
    if(ret!=0)
    {
        perror("pthread_create");
        printf("error:%d\n",ret);
        return -1;
    }
  //sleep(1);
  //pthread_cancel(tid);
    pthread_join(tid,NULL);
    pthread_join(tid2,NULL);
    pthread_join(tid3,NULL);
    pthread_join(tid4,NULL);
    while(1)
    {
        printf("I am running\n");
        sleep(1);
    }
    return 0;
}


