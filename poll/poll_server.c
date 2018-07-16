#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define MAX 256

void InitPoll(struct pollfd* list,int size)
{
    int i=0;
    for(;i<size;i++)
    {
        list[i].fd=-1;
        list[i].events=0;
        list[i].revents=0;
    }
}

void AddPoll(int fd,struct pollfd* list,int size)
{
    int i=0;
    for(;i<size;i++)
    {
        if(list[i].fd==-1)
        {
            list[i].fd=fd;
            list[i].events=POLLIN;
            return;
        }
    }
}


int startup(int port)
{
    int sock=socket(AF_INET,SOCK_STREAM,0);
    if(sock<0)
    {
        perror("socket");
        return 2;
    }

    int opt=1;
    setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    struct sockaddr_in local;
    local.sin_family=AF_INET;
    local.sin_addr.s_addr=(INADDR_ANY);
    local.sin_port=htons(port);

    if(bind(sock,(struct sockaddr*)&local,sizeof(local))<0)
    {
        perror("bind");
        return 3;
    }

    if(listen(sock,5)<0)
    {
        perror("listen");
        return 4;
    }
    return sock;
}

void ServiceIO(struct pollfd* list,int size,int listen_sock)
{
    int i=0;
    for(;i<size;i++)
    {
        if(list[i].fd==-1)
            continue;
        if(!(list[i].revents & POLLIN))
            continue;
        if(list[i].fd==listen_sock&&(list[i].events&POLLIN))
        {
            struct sockaddr_in client;
            socklen_t len=sizeof(client);
            int newsock=accept(listen_sock,(struct sockaddr*)&client,&len);
            if(newsock<0)
            {
                perror("accept");
                continue;
            }
            printf("get a connect:[%s:%d]\n",\
                       inet_ntoa(client.sin_addr),ntohs(client.sin_port));
            AddPoll(newsock,list,MAX);
        }
        else
        {
            char buf[10240];
            ssize_t s=read(list[i].fd,buf,sizeof(buf)-1);
            if(s<0)
            {
                perror("read");
                close(list[i].fd);
                list[i].fd=-1;
            }
            else if(s==0)
            {
                printf("client quit!\n");
                close(list[i].fd);
                list[i].fd=-1;
            }
            else
            {
                buf[s]=0;
                printf("client say:%s\n",buf);
            }
        }
    }
}


int main(int argc,char* argv[])
{
    if(argc!=2)
    {
        printf("Usage:%s [port]\n",argv[0]);
        return 1;
    }

    int listen_sock=startup(atoi(argv[1]));
    
    struct pollfd poll_list[MAX];
    int size=sizeof(poll_list)/sizeof(struct pollfd);
    InitPoll(poll_list,size);
    AddPoll(listen_sock,poll_list,size);

    for(;;)
    {
        switch(poll(poll_list,size,10000))
        {
        case -1:
            perror("poll");
            break;
        case 0:
            printf("timeout...\n");
            break;
        default:
            ServiceIO(poll_list,size,listen_sock);
            break;
        }
    }

}
