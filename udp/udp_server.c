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
        printf("Usage:%s ip port\n",argv[0]);
        return 1;
    }
    //创建套接字
    int sock=socket(AF_INET,SOCK_DGRAM,0);
    if(sock<0)
    {
        perror("socket");
        return -1;
    }
    printf("create socket ok\n");
    //初始化bind的参数
    struct sockaddr_in local;
    local.sin_family=AF_INET;
    local.sin_port=htons(atoi(argv[2]));
    local.sin_addr.s_addr=inet_addr(argv[1]);
    //绑定
    int b=bind(sock,(struct sockaddr*)&local,sizeof(local));
    if(b<0)
    {
        perror("bind");
        return -2;
    }
    printf("bind ok\n");
    
    //发送接收消息
    char buf[521];
    while(1)
    {
        socklen_t len=sizeof(local);
        ssize_t s=recvfrom(sock,buf,sizeof(buf)-1,0,(struct sockaddr*)&local,&(len));
        if(s>0)
        {
            buf[s]=0;
            if(strcmp(buf,"quit")==0)
            {
                printf("client quit!\n");
                break;
            }
            printf("[%s:%d]:%s\n",inet_ntoa(local.sin_addr),ntohs(local.sin_port),buf);
            sendto(sock,buf,strlen(buf),0,(struct sockaddr*)&local,len);
        }
    }

    return 0;
}
