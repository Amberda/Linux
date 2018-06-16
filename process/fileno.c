#include <stdio.h>


int main()
{
    printf("%d\n",fileno(stdin));
    printf("%d\n",fileno(stdout));
}
