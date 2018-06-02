#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>


int main()
{
    int wfd = 0;
    if((wfd = open("mypipe",O_WRONLY))==-1)
    {
        perror("open");
        return 1;
    }
    char buf[1024];
    while(1)
    {
        printf("Please Enter:");
        //从标准输入读取
        fflush(stdout);
        ssize_t s = read(0,buf,sizeof(buf)-1);
        if(s>0)
        {
            buf[s]=0;
            write(wfd,buf,strlen(buf));
        }
        else if(s==-1)
        {
            perror("write");
            return 2;
        }
    }
    return 0;
}
