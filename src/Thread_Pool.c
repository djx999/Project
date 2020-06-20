#include<Thread_Pool.h>


pool_t* create(int min,int max,int quemax)
{
	pool_t* p = NULL;
	if((p = (pool_t*)malloc(sizeof(pool_t)))==NULL)
		return NULL;
	p->thread_min = min;
	p->thread_max = max;
	p->thread_busy = 0;
	p->thread_alive = 0;
	p->thread_shutdown = 1;
	p->thread_exit = 0;
	if((p->tid = (pthread_t*)malloc(sizeof(pthread_t)*max))==NULL)
		return NULL;
	bzero(p->tid,sizeof(pthread_t)*max);
	if((p->queue = (task_t*)malloc(sizeof(task_t)*quemax))==NULL)
		return NULL;
	p->queue_front = 0;
	p->queue_rear = 0;
	p->queue_size =0;
	p->queue_max = quemax;
	if((pthread_mutex_init(&p->lock,NULL)!=0)||((pthread_cond_init(&p->customer,NULL))!=0)||((pthread_cond_init(&p->create,NULL))!=0))
		return NULL;
	for(int i=0;i<min;i++)
	{
		pthread_create(&p->tid[i],NULL,customer,(void*)p);
		++p->thread_alive;
	}
	pthread_create(&p->managerid,NULL,manager,(void*)p);
	printf("thread pool create..\n");
	return p;
}
int addtask(pool_t *p,task_t task)
{
	pthread_mutex_lock(&p->lock);
	if(p->thread_shutdown)
	{
		while(p->queue_size == p->queue_max)
		{
			pthread_cond_wait(&p->create,&p->lock);
		}
		p->queue[p->queue_front].job = task.job;
		p->queue[p->queue_front].arg = task.arg;
		p->queue_front = (p->queue_front+1)%(p->queue_max);
		++p->queue_size;
		pthread_mutex_unlock(&p->lock);
		pthread_cond_signal(&p->customer);
		return 0;
	}
	else
	{
		pthread_mutex_unlock(&p->lock);
		return -1;
	}

}
void* customer(void* arg)
{
	pool_t *p = (pool_t*)arg;
	while(p->thread_shutdown)
	{
		pthread_mutex_lock(&p->lock);
		while(p->queue_size ==0)
		{
			pthread_cond_wait(&p->customer,&p->lock);
			if(p->thread_exit >0)
				break;
		}
		if(p->thread_shutdown !=1 || p->thread_exit >0)
		{
			--p->thread_alive;
			--p->thread_exit;
			pthread_mutex_unlock(&p->lock);
			pthread_exit(NULL);
		}
		//取任务 取完解锁
		task_t task;
		task.job = p->queue[p->queue_rear].job;
		task.arg = p->queue[p->queue_rear].arg;
		p->queue_rear = (p->queue_rear+1)%(p->queue_max);
		--p->queue_size;
		++p->thread_busy;
		//解锁执行任务
		pthread_mutex_unlock(&p->lock);
		//唤醒消费者
		pthread_cond_signal(&p->customer);
		printf("customer tid job :0x%x\n",(unsigned int)pthread_self());
		(task.job)(task.arg);
		//执行任务过后
		pthread_mutex_lock(&p->lock);
		--p->thread_busy;
		pthread_mutex_unlock(&p->lock);
	}
	pthread_exit(NULL);
}
int thread_if_alive(pthread_t tid)
{
	pthread_kill(tid,0);
	if(errno == ESRCH)
		return 0;
	else
		return 1;
}

void* manager(void* arg)
{
	pool_t *p= (pool_t*)arg;
	int add = 0;
	int quesize;
	int alive;
	int busy;
	int i = 0;
	while(p->thread_shutdown)
	{
		pthread_mutex_lock(&p->lock);
		quesize = p->queue_size;
		alive = p->thread_alive;
		busy = p->thread_busy;
		pthread_mutex_unlock(&p->lock);
		printf("alive busy idea quesize:%d %d %d %d\n",alive,busy,alive-busy,quesize);
		if((quesize > alive-busy ||(double)busy/alive *100 >=(double)70) && alive+p->thread_min <= p->thread_max)
		{
			for(i;i<p->thread_max&& add<p->thread_min;i++)
			{
				if(p->tid[i] == 0 || !thread_if_alive(p->tid[i]))
				{
					pthread_create(&p->tid[i],NULL,customer,(void*)p);
					add++;
					pthread_mutex_lock(&p->lock);
					++p->thread_alive;
					pthread_mutex_unlock(&p->lock);
				}
			}
		}
		if(busy *2 < alive-busy && alive - p->thread_min >= p->thread_min)
		{
			pthread_mutex_lock(&p->lock);
			p->thread_exit = p->thread_min;
			pthread_mutex_unlock(&p->lock);
			for(int i=0;i<p->thread_min;i++)
			{
				pthread_cond_signal(&p->customer);
			}
		}
		sleep(3);
	}
	pthread_exit(NULL);
}
void destroy(pool_t *p)
{
	p->thread_shutdown = 0;
	free(p->tid);
	free(p->queue);
	pthread_mutex_destroy(&p->lock);
	pthread_cond_destroy(&p->customer);
	pthread_cond_destroy(&p->create);
}

