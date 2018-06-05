
#include "comm.h"

static int Msg(int flag)
{
    key_t key=ftok(".",0x6666);
    if(key<0)
    {
        perror("key");
        return -1;
    }
    int msqid=msgget(key,flag);
    if(msqid<-1)
    {
        perror("msgget");
        return -2;
    }
    return msqid;
}




int CreatMsg()
{
    return Msg(IPC_CREAT|IPC_EXCL|0666);
}
int GetMsg()
{
    return Msg(IPC_CREAT);
}
int DestoryMsg(int msqid )
{
    if(msgctl(msqid,IPC_RMID,NULL)<0)
    {
        perror("msgctl");
        return -1;
    }
    return 0;
}
int SendMsg(int msqid,long sndtype,char* msg)
{
    struct msgbuf buf;
    buf.mtype=sndtype;
    strcpy(buf.mtext,msg);
    if(msgsnd(msqid,(void*)&buf,sizeof(buf.mtext),0)<0)
    {
        perror("msgsnd");
        return -1;
    }
    return 0;
}
int RecvMsg(int msqid,int rcvtype,char* buf)
{
    struct msgbuf buff;
    int n = msgrcv(msqid,(void*)&buff,sizeof(buff.mtext),rcvtype,0);
    if(n<0)
    {
        perror("msgrcv");
        return -1;
    }
    strncpy(buf,buff.mtext,n);
    return 0;
}

