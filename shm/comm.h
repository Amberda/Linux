#ifndef _COMM_H__
#define _COMM_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int CreatShm(int size);
int DestroyShm(int shmid);
int GetShm(int size);

#endif

