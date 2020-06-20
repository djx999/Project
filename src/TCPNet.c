#include<TCPNet.h>
#include<Thread_Pool.h>
#include<TcpKernel.h>
#include<netinet/tcp.h>
#include<netinet/in.h>
int InitNet()
{
	struct sockaddr_in addr;
	bzero(&addr,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8000);
	inet_pton(AF_INET,"192.168.1.103",&addr.sin_addr.s_addr);

	int serverfd = socket(AF_INET,SOCK_STREAM,0);
	if(serverfd <0)
		printf("serverfd fail..\n");
	//新增
	int re=1;
	setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&re, re);
	setsockopt(serverfd, SOL_SOCKET, SO_REUSEPORT, (const void *)&re, re);
	if(setsockopt(serverfd, SOL_SOCKET, SO_KEEPALIVE,&re, sizeof(int)))
	{
		perror("");
		exit(0);
	}

	bind(serverfd,(struct sockaddr*)&addr,sizeof(addr));
	listen(serverfd,128);

	return serverfd;

}
void UnInitNet()
{
	pthread_mutex_destroy(&acclock);
}
void* Job(void* arg)
{
	int serverfd = *(int*)arg;
	int clientfd;
	struct sockaddr_in clientaddr;
	int nSize = sizeof(clientaddr);
	pthread_mutex_init(&acclock,NULL);
	pthread_mutex_lock(&acclock);
	clientfd = accept(serverfd,(struct sockaddr*)&clientaddr,&nSize);
	pthread_mutex_unlock(&acclock);
	if(clientfd <0)
		printf("clientfd fail ..\n");

	int nlen;
	int packsize;
	char *pszbuf = NULL;
	while(1)
	{
		//先收包大小
		nlen = recv(clientfd,(char*)&packsize,sizeof(int),0);
		if(nlen <=0)
		{
			close(clientfd);
			//continue;
			break;
		}
		//printf("接收成功..\n");
		//再收包内容
		pszbuf = (char*)malloc(packsize);
		int offset = 0;
		while(packsize)
		{
			nlen = recv(clientfd,pszbuf+offset,packsize,0);
			//printf("内容接收成功大小为：%d\n",nlen);
			packsize-=nlen;
			offset+=nlen;
		}
		//处理
		DealData(clientfd,pszbuf);
		//RegisterRq(clientfd,pszbuf);
		free(pszbuf);
		pszbuf = NULL;
	}

}
void EpollNet()
{

	pool_t *p;
	task_t task;
	int serverfd = InitNet();
	task.job = Job;
	task.arg = (void*)&serverfd;
	int ready;
	int epfd;
	struct epoll_event readyarr[1];
	struct epoll_event temp;
	epfd = epoll_create(1);
	temp.data.fd = serverfd;
	temp.events = EPOLLIN|EPOLLET;
	epoll_ctl(epfd,EPOLL_CTL_ADD,serverfd,&temp);
	p = create(10,300,500);	
	if(p == NULL)
	{
		printf("thread pool create fail..\n");
		exit(0);
	}
	while(p->thread_shutdown)
	{
		if((ready = epoll_wait(epfd,readyarr,1,-1))>0)
		{
			addtask(p,task);
		}
	}
}

int SendData(unsigned int sock,char* szbuf,int nlen)
{
	if(send(sock,(char*)&nlen,sizeof(int),0)<0)
		return -1;
	if(send(sock,szbuf,nlen,0)<0)
		return -1;

	return 0;
		

}
