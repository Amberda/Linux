#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main()
{
    mkfifo("mypipe",0644);
    int infd = open("1.txt",O_RDONLY);
    if(infd==-1)
    {
        perror("open");
        return 1;
    }
    int outfd=open("mypipe",O_WRONLY);
    if(outfd==-1)
    {
        perror("open");
        return 2;
    }
    char buf[1024];
    int n;
    while((n=read(infd,buf,1024))>0)
    {
        write(outfd,buf,n);
    }
    close(infd);
    close(outfd);
    return 0;
}
