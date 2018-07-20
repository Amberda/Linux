#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

//op=0  +
//op=1  -
//op=2  *
//op=3  /
//op=4  %

typedef struct {
    int x;
    int y;
    char op;
}Request_t;


typedef struct{
    int num;
}Response_t;
