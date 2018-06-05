#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <string.h>

struct msgbuf
{
    long mtype;
    char mtext[1024];
};

#define CLIENT_TYPE 1
#define SERVER_TYPE 2

int CreatMsg();
int GetMsg();
int DestoryMsg(int msgid );
int SendMsg(int msgid,long sndtype,char* msg);
int RecvMsg(int msgid,int rcvtype,char* buf);

