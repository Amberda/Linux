#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>


int main()
{
    if(mkfifo("mypipe",0644)==-1)
    {
        perror("mkfifo");
        return 1;
    }
    int rfd = 0;
    if((rfd = open("mypipe",O_RDONLY))==-1)
    {
        perror("open");
        return 2;
    }
    char buf[1024];
    while(1)
    {
        ssize_t s = read(rfd,buf,sizeof(buf)-1);
        if(s>0)
        {
            buf[s-1]=0;
            printf("client say:%s\n",buf);
        }
        else if(s==0)
        {
            printf("client quit!!!\n");
            break;
        }
        else
        {
            perror("read");
            return 3;
        }
    }
    return 0;
}
