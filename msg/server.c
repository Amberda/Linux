#include "comm.h"

int main()
{
    int msqid=CreatMsg();
    char buf[1024];
    while(1)
    {
        buf[0]=0;
        RecvMsg(msqid,CLIENT_TYPE,buf);
        printf("client say :%s\n",buf);

        printf("Please Enter:");
        fflush(stdout);
        ssize_t s=read(0,buf,sizeof(buf));
        if(s>0)
        {
            buf[s-1]=0;
            SendMsg(msqid,SERVER_TYPE,buf);
        }
    }
    DestoryMsg(msqid);
    return 0;
}
