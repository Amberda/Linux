
#include "comm.h"

static int Shm(int size,int flag)
{
    key_t key=ftok(".",0x6666);
    if(key<0)
    {
        perror("ftok");
        return -1;
    }
    int shmid=shmget(key,size,flag);
    if(shmid<0)
    {
        perror("shmget");
        return -2;
    }
    return shmid;
}
int CreatShm(int size)
{
    return Shm(size,IPC_CREAT|IPC_EXCL|0666);
}
int GetShm(int size)
{
    return Shm(size,IPC_CREAT);
}
int Destroy(int shmid)
{
    if(shmctl(shmid,IPC_RMID,NULL)<0)
    {
        perror("shmctl");
        return -1;
    }
    return 0;
}
