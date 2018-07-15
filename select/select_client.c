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
        printf("Usage %s ip port\n",argv[0]);
        return 1;            
    }
    int sock=socket(AF_INET,SOCK_STREAM,0);
    if(sock<0)
    {
        perror("socket");
        return 2;        
    }

    struct sockaddr_in server;
    server.sin_family=AF_INET;
    server.sin_port=htons(atoi(argv[2]));
    server.sin_addr.s_addr=inet_addr(argv[1]);

    if(connect(sock,(struct sockaddr*)&server,sizeof(server))<0)
    {
            perror("connect");
            return 3;    
    }
    while(1)
    {
        char buf[1024];
        buf[0]=0;
        printf("Please Enter:");
        fflush(stdout);
        ssize_t s=read(0,buf,sizeof(buf)-1);
        if(s>0)
        {
            buf[s]=0;        
        }
        write(sock,buf,strlen(buf));
        if(strncmp(buf,"quit",4)==0)
        {
            break;
        }
    }
    close(sock);
    return 0;
}                                                               
