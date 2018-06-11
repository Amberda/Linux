#include "comm.h"

int main()
{
    int shmid=CreatShm(1024);
    printf("creat shm ok!\n");
    char* msg=shmat(shmid,NULL,0);
    sleep(2);
    int i=0;
    while(i++<10)
    {
        printf("client say:%s\n",msg);
        sleep(1);
    }    
    shmdt(msg);
    sleep(5);
    Destroy(shmid);
    return 0;
}
