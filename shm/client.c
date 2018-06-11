#include "comm.h"

int main()
{
    int shmid=GetShm(1024);
    sleep(1);
    char* msg=shmat(shmid,NULL,0);
    sleep(2);
    int i=0;
    while(i<10)
    {
        msg[i]='0'+i;
        i++;
        msg[i]=0;
        sleep(1);
    }
    shmdt(msg);
    return 0;
}
