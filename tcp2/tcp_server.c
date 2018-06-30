#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

//多进程版本

int main(int argc,char* argv[])
{
    if(argc!=3)
    {
        printf("Usage %s ip port\n",argv[0]);
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
        struct sockaddr_in peer;
        char buf[1024];
        buf[0]=0;
        socklen_t len=sizeof(peer);
        int newsock=accept(sock,(struct sockaddr *)&peer,&len);
        if(newsock<0)
        {
            perror("accept");
            return 5;
        }
        inet_ntop(AF_INET,&peer.sin_addr,buf,sizeof(buf));
        printf("get a connect,ip:%s,port:%d\n",buf,ntohs(peer.sin_port));
    
        pid_t id=fork();
        if(id<0)
        {
            perror("fork");
            return -1;
        }
        else if(id==0)
        {//child
            if(fork()==0)
            {//grand_child
                while(1)
                {
                    ssize_t s=read(newsock,buf,sizeof(buf));
                    if(s>0)
                    {
                        buf[s]=0;
                        printf("[%s:%d] %s",inet_ntoa(peer.sin_addr),ntohs(peer.sin_port),buf);
                    }
                    else
                    {
                        printf("client quit\n");
                        close(newsock);
                        break;
                    }
                    write(newsock,buf,strlen(buf));
                }
            }
            exit(0);
        }
        else
        {//father
            close(newsock);
            waitpid(id,NULL,0);
        }
    }
    close(sock);
    return 0;
}
