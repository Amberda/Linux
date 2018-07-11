#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main(int argc,char* argv[])
{
    if(argc!=3)
    {
        printf("Usage:%s ip port",argv[0]);
        return 1;
    }
    //创建套接字
    int sock=socket(AF_INET,SOCK_DGRAM,0);
    if(sock<0)
    {
        perror("socket");
        return -1;
    }
    struct sockaddr_in local;
    local.sin_family=AF_INET;
    local.sin_port=htons(atoi(argv[2]));
    local.sin_addr.s_addr=inet_addr(argv[1]);
    
    //发送接收消息
    char buf[521];
    struct sockaddr_in peer;
    while(1)
    {
        printf("Please Enter:");
        fflush(stdout);
        socklen_t len=sizeof(peer);
        ssize_t s=read(0,buf,sizeof(buf)-1);
        if(s>0)
        {
            buf[s-1]=0;
            sendto(sock,buf,strlen(buf),0,(struct sockaddr*)&local,len);
            if(strcmp(buf,"quit")==0)
            {
                break;
            }
            ssize_t num=recvfrom(sock,buf,sizeof(buf)-1,0,(struct sockaddr*)&local,&(len));
            if(num>0)
            {
                buf[s]=0;
                printf("server echo:%s\n",buf);
            }
        }
    }
    return 0;
}
