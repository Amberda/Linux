#include "comm.h"

int main()
{
    int msqid=GetMsg();
    char buf[1024];
    while(1)
    {
        buf[0]=0;
        printf("Please Enter:");
        fflush(stdout);
        ssize_t s = read(0,buf,sizeof(buf));
        if(s<0)
        {
            perror("read");
            return -1;
        }
        if(s>0)
        {
            buf[s-1]=0;
            SendMsg(msqid,CLIENT_TYPE,buf);
        }
        RecvMsg(msqid,SERVER_TYPE,buf);
        printf("server say:%s\n",buf);
    }
    return 0;
}
