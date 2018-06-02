#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


int main()
{
    int fd[2] = {0};
    if(pipe(fd)==-1)
    {
        perror("pipe");
        return 1;
    }
    pid_t pid = fork();
    if(pid==-1)
    {
        perror("fork");
        return 2;
    }
    else if(pid==0)
    {//child write
        close(fd[0]);
        char *msg="father,I am child";
        int i = 5;
        while(i--)
        {
            write(fd[1],msg,strlen(msg));
            sleep(5);
        }
    }
    else
    {//father read
        close(fd[1]);
        char buf[1024];
        while(1)
        {
            ssize_t s = read(fd[0],buf,sizeof(buf));
            if(s>0)
            {
                buf[s]=0;
                printf("father: %s\n",buf);
            }
            else if(s==0)
            {
                printf("child quit!!!\n");
                break;
            }
            else
            {
                perror("read");
                return 3;
            }
        }
    }
    return 0;
}
