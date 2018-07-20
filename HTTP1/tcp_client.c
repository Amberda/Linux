
#include "comm.h"

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
        Request_t req;
        Response_t res;
        printf("Please Enter:");
        scanf("%d %c %d",&req.x,&req.op,&req.y);
        fflush(stdout);

        write(sock,&req,sizeof(req));

        read(sock,&res,sizeof(res));
        //fflush(stdout);
        printf("server echo:%d\n",res.num);
    }
    close(sock);
    return 0;
}
