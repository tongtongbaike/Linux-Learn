#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<string.h>
#define _PATH_ "."
#define _PROJ_ID_ 0x757
#define _SIZE_ 4*1024
static int shm(size_t size,int flags);
int creat_shm(size_t size);
int get_shm(size_t size);
void* at_shm(int shm_id);
int dt_shm(const void* shmadd);
int destory_shm(int shm_id);

