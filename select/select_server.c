#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_SET sizeof(fd_set)*8
#define INIT_DATA -1


int startup(int port)
{
    int sock=socket(AF_INET,SOCK_STREAM,0);
    if(sock<0)
    {
        perror("sock");
        exit(2);
    }

    int opt=1;
    setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    struct sockaddr_in local;
    local.sin_family=AF_INET;
    local.sin_addr.s_addr=htonl(INADDR_ANY);
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

void InitArray(int arr[],int num)
{
    int i=0;
    for(;i<num;i++)
    {
        arr[i]=INIT_DATA;
    }
}

int AddSockToArray(int fd,int arr[],int num)
{
    int i=0;
    for(;i<num;i++)
    {
        if(arr[i]==INIT_DATA)
        {
            arr[i]=fd;
            return i;
        }
    }
    return -1;
}

int SetArrayToFdSet(int arr[],int num,fd_set* rfds)
{
    int max=INIT_DATA;
    int i=0;
    for(;i<num;i++)
    {
        if(arr[i]!=-1)
        {
            FD_SET(arr[i],rfds);
        }
        if(arr[i]>max)
        {
            max=arr[i];
        }
    }
    return max;
}

void serviceIO(int arr[],int num,fd_set* rfds)
{
    int i=0;
    for(;i<num;i++)
    {
        if(arr[i]>INIT_DATA)
        {
            int fd=arr[i];
            if(i==0&&FD_ISSET(arr[i],rfds))
            {
                struct sockaddr_in client;
                socklen_t len=sizeof(client);
                int newsock=accept(fd,(struct sockaddr*)&client,&len);
                if(newsock<0)
                {
                    perror("accept");
                    continue;
                }
                printf("get a connect:[%s:%d]\n",inet_ntoa(client.sin_addr),ntohs(client.sin_port));
                int ret = AddSockToArray(newsock,arr,num);
                if(ret==-1)//full
                {
                    close(newsock);
                }
            }
            else if(i>0&&FD_ISSET(arr[i],rfds))
            {
                char buf[1024];
                ssize_t s=read(arr[i],buf,sizeof(buf)-1);
                if(s>0)
                {
                    buf[s]=0;
                    printf("client say:%s",buf);
                    fflush(stdout);
                }
                else if(s==0)
                {
                    printf("client quit!\n");
                    close(arr[i]);
                    arr[i]=INIT_DATA;
                }
                else
                {
                    perror("read");
                    close(arr[i]);
                    arr[i]=INIT_DATA;
                }
            }
            //else
            //{
            //   break;
            //}
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
    
    int fd_array[MAX_SET];
    InitArray(fd_array,MAX_SET);

    AddSockToArray(listen_sock,fd_array,MAX_SET);

    fd_set rfds;
    for(;;)
    {
        FD_ZERO(&rfds);
        int max_fd = SetArrayToFdSet(fd_array,MAX_SET,&rfds);
        switch(select(max_fd+1,&rfds,NULL,NULL,NULL))
        {
        case -1:
            perror("select");
            break;
        case 0:
            printf("timeout...\n");
            break;
        default:
            serviceIO(fd_array,MAX_SET,&rfds);
            break;
        }
    }
    return 0;
}
