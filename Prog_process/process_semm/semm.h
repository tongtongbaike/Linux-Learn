#pragma once

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>
#include <errno.h>

#define _KEY_PATH "../"
#define PROJECT_ID 0x57

union semun
{
	int val;
	struct semid_ds *buf;
	unsigned short* arry;
	struct seminfo* _buf;
};

int creat_sem_set(int _sem_nums);
int get_sem_set(int _sem_nums);
int init_sem_set(int _semid,int _seq_num,int _init_val);
int p_sem_elem(int _sem_id,int seq_num);
int v_sem_elem(int _sem_id,int seq_num);
int destory(int _sem_id);
