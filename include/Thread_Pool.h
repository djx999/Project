#include<pthread.h>
#include<signal.h>
#include<errno.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

typedef struct node
{
	void* (*job)(void*);
	void* arg;

}task_t;


typedef struct node1
{
	int thread_min;
	int thread_max;
	int thread_busy;
	int thread_alive;
	int thread_shutdown;
	pthread_t *tid;
	task_t *queue;
	pthread_mutex_t lock;
	pthread_cond_t  customer;
	pthread_cond_t  create;
	int queue_front;
	int queue_rear;
	int queue_size;
	int queue_max;
	int thread_exit;
	pthread_t managerid;
	

}pool_t;
pool_t* create(int,int,int);
int     addtask(pool_t*,task_t);
void*   customer(void*);
void*   manager(void*);
void*   userjob(void*);
void    destroy(pool_t*);
int     thread_if_alive(pthread_t);

