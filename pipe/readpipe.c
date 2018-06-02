#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main()
{
    int outfd=open("2.txt",O_WRONLY|O_CREAT|O_TRUNC,0644);
    if(outfd==-1)
    {
        perror("open");
        return 1;
    }
    int infd=open("mypipe",O_RDONLY);
    if(infd==-1)
    {
        perror("open");
        return 2;
    }
    char buf[1024];
    ssize_t s;
    while((s=read(infd,buf,1024))>0)
    {
        write(outfd,buf,s);
    }
    close(infd);
    close(outfd);
    return 0;
}
