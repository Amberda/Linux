#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


int main()
{
    //char *argv[]={"ls","-l",NULL};
    char *envp[]={"year=2018","name=lubaobao",NULL};
    if(execlp("./envp","envp",NULL,envp)==-1)
    {
        perror("execv");
        exit(1);
    }
    //execlp()
}
