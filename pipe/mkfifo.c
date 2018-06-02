#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

int main()
{
    if(mkfifo("fifo",0644)==-1)
    {
        perror("mkfifo");
        return 1;
    }
    return 0;
}
