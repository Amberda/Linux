#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX 256

int startup(int port)
{
    int sock=socket(AF_INET,SOCK_STREAM,0);
    if(sock<0)
    {
        perror("socket");
        exit(2);
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
        exit(3);
    }

    if(listen(sock,5)<0)
    {
        perror("listen");
        exit(4);
    }

    return sock;
}

int AddEvent(int epfd,int opt,int fd,int events)
{
    struct epoll_event ev;
    ev.events=events;
    ev.data.fd=fd;
    int ret = epoll_ctl(epfd,opt,fd,&ev);
    return ret;
}

void ServiceIO(int listen_sock,int epfd,struct epoll_event* ev,int size)
{
    int i=0;
    struct epoll_event evwrite;
    for(;i<size;i++)
    {
        int fd=ev[i].data.fd;
        if(ev[i].events&EPOLLIN)
        {
            if(fd==listen_sock)
            {
                struct sockaddr_in client;
                socklen_t len=sizeof(client);
                int newsock=accept(fd,(struct sockaddr*)&client,&len);
                if(newsock<0)
                {
                    perror("accept");
                    continue;
                }
                printf("get a connect [%s:%d]\n",\
                       inet_ntoa(client.sin_addr),ntohs(client.sin_port));

                if((AddEvent(epfd,EPOLL_CTL_ADD,newsock,EPOLLIN))<0)
                {
                    perror("epoll_ctl");
                    return;
                }
            }
            else
            {
                char buf[10240];
                ssize_t s = read(fd,buf,sizeof(buf)-1);
                if(s<0)
                {
                    perror("read");
                    close(fd);
                    epoll_ctl(epfd,EPOLL_CTL_DEL,fd,NULL);
                }
                else if(s==0)
                {
                    printf("client quit!\n");
                    close(fd);
                    epoll_ctl(epfd,EPOLL_CTL_DEL,fd,NULL);
                }
                else
                {
                    buf[s]=0;
                    printf("client say:%s\n",buf);

                    evwrite.events=EPOLLOUT;
                    evwrite.data.fd=fd;
                    epoll_ctl(epfd,EPOLL_CTL_MOD,fd,&evwrite);
                }
            }
        }
        if(ev[i].events&EPOLLOUT)
        {
            const char* msg="HTTP/1.0 200 OK\r\n\r\n<html><h1>hello lubaobao:)</h1></html>\r\n";
            write(fd,msg,strlen(msg));
            close(fd);
            epoll_ctl(epfd,EPOLL_CTL_DEL,fd,NULL);
        }
    }
}

int main(int argc,char* argv[])
{
    if(argc!=2)
    {
        printf("Usage: %s [port]\n",argv[0]);
        return 1;
    }

    int listen_sock=startup(atoi(argv[1]));

    int epfd=epoll_create(MAX);
    if(epfd<0)
    {
        perror("epoll_create");
        return 5;
    }

    struct epoll_event ev;
    ev.events=EPOLLIN;
    ev.data.fd=listen_sock;
    if((epoll_ctl(epfd,EPOLL_CTL_ADD,listen_sock,&ev))<0)
    {
        perror("epoll_ctl");
        return 6;
    }

    for(;;)
    {
        struct epoll_event eplist[MAX];
        int num=epoll_wait(epfd,eplist,MAX,-1);
        switch(num)
        {
        case -1:
            perror("epoll_wait");
            break;
        case 0:
            printf("timeout...\n");
            break;
        default:
            ServiceIO(listen_sock,epfd,eplist,num);
            break;
        }
    }
    return 0;
}
