#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>

union semun
{
    int val;
};

int CreatSem(int nums)
{
    key_t key=ftok(".",0x6666);
    if(key<0)
    {
        perror("ftok");
        return -1;
    }
    int semid=semget(key,nums,IPC_CREAT|IPC_EXCL|0666);
    if(semid<0)
    {
        perror("semget");
        return -2;
    }
    return semid;
}
int GetSem(int nums)
{
   key_t key=ftok(".",0x6666);
    if(key<0)
    {
        perror("ftok");
        return -1;
    }
    int semid=semget(key,nums,IPC_CREAT);
    if(semid<0)
    {
        perror("semget");
        return -2;
    }
    return semid;
}
int InitSem(int semid,int nums,int value)
{
    union semun su;
    su.val=value;
    int s=semctl(semid,nums,SETVAL,su);
    if(s<0)
    {
        perror("semctl");
        return -1;
    }
    return 0;
}
int SemPV(int semid,int num,int op)
{
    struct sembuf buf;
    buf.sem_num=num;
    buf.sem_op=op;
    buf.sem_flg=0;
    int sp=semop(semid,&buf,1);
    if(sp<0)
    {
        perror("semop");
        return -1;
    }
    return 0;
}
int P(int semid,int num)
{
    return SemPV(semid,num,-1);
}
int V(int semid,int num)
{
    return SemPV(semid,num,1);
}
int DestroySem(int semid)
{
    int sc=semctl(semid,0,IPC_RMID);
    if(sc<0)
    {
        perror("semctl");
        return -1;
    }
    return 0;
}

int main()
{
    int semid=CreatSem(1);
    InitSem(semid,0,1);
    pid_t pid=fork();
    if(pid<0)
    {
        perror("fork");
        return -1;
    }
    if(pid==0)
    {
        int sem_id=GetSem(1);
        while(1)
        {
            P(sem_id,0);
            printf("O");
            fflush(stdout);
            usleep(329471);
            printf("O ");
            fflush(stdout);
            usleep(123745);
            V(semid,0);
        }
    }
    else
    {
        while(1)
        {
            P(semid,0);
            printf("X");
            fflush(stdout);
            usleep(223456);
            printf("X ");
            fflush(stdout);
            usleep(341622);
            V(semid,0);
        }
    }
    DestroySem(semid);
    return 0;
}
