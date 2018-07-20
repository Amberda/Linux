
#include "comm.h"

//多线程版本   
typedef struct
{
    int fd;
    char ip[32];
    int port;
}Arg;

void server(int fd,char* ip,int port)
{
    Request_t req;
    Response_t res;
    while(1)
    {
        ssize_t s=read(fd,&req,sizeof(req));
        if(s>0)
        {
            if(req.op=='+')
            {
                res.num=req.x+req.y;
                printf("[%s:%d] %d + %d = %d\n",ip,port,req.x,req.y,res.num);
            }
            if(req.op=='-')
            {
                res.num=req.x-req.y;
                printf("[%s:%d] %d - %d = %d\n",ip,port,req.x,req.y,res.num);
            }
            if(req.op=='x')
            {
                res.num=req.x*req.y;
                printf("[%s:%d] %d x %d = %d\n",ip,port,req.x,req.y,res.num);
            }
            if(req.op=='/')
            {
                res.num=req.x/req.y;
                printf("[%s:%d] %d / %d = %d\n",ip,port,req.x,req.y,res.num);
            }
            if(req.op=='%')
            {
                res.num=req.x%req.y;
                printf("[%s:%d] %d %% %d = %d\n",ip,port,req.x,req.y,res.num);
            }
        }
        else
        {
            printf("client quit!\n");
            close(fd);
            break;
        }
        write(fd,&res,sizeof(res));
    }
}


void* thread_server(void* arg)
{
    char buf[1024]={0};
    Arg* p=(Arg*)arg;
    server(p->fd,p->ip,p->port);
    free(p);
    return NULL;
}
    
int main(int argc,char* argv[])
{
    if(argc!=3)
    {
        printf("Usage:%s ip port",argv[0]);
        return 1;
    }
    int sock=socket(AF_INET,SOCK_STREAM,0);
    if(sock<0)
    {
        perror("socket");
        return 2;
    }

    struct sockaddr_in local;
    local.sin_family=AF_INET;
    local.sin_port=htons(atoi(argv[2]));
    local.sin_addr.s_addr=inet_addr(argv[1]);

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
    while(1)
    {
        char buf[1024];
        buf[0]=0;
        struct sockaddr_in peer;
        socklen_t len=sizeof(peer);
        int newsock=accept(sock,(struct sockaddr *)&peer,&len);
        if(newsock<0)
        {
            perror("accept");
            return 5;
        }
        inet_ntop(AF_INET,&peer.sin_addr,buf,sizeof(buf));
        printf("get a connect,ip:%s,port:%d\n",buf,ntohs(peer.sin_port));

        pthread_t tid=0;
        Arg* arg=(Arg*)malloc(sizeof(Arg));
        arg->fd=newsock;
        strcpy(arg->ip,buf);
        arg->port=ntohs(peer.sin_port);
        pthread_create(&tid,NULL,thread_server,(void*)arg);
        pthread_detach(tid);
    }
    close(sock);
    return 0;
}
