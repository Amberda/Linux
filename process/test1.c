#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void myexecv(const char* path,const char* argv[])
{
    extern char** environ;
    return execve(path,argv,environ);
}

int main()
{
    int i;
 //   for(i=0;environ[i]!=NULL;i++)
 // {
 //   printf("%s\n",environ[i]);
 // }
    char *const argv[]={"envp",NULL};
    myexecv("./envp",argv);
}
