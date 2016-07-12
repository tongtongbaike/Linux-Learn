#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define _PATH_ "."
#define _PROJECT_ 0x777
#define _SHM_SIZE_ 4*1024

int get_shm();
void* at_shn();
int delete_shm();
int rm_shm();


