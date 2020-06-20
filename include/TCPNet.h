#include<sys/socket.h>
#include<sys/epoll.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<ctype.h>
#include<pthread.h>

pthread_mutex_t  acclock;


int InitNet();
void UnInitNet();
void* Job(void* arg);
void EpollNet();
int SendData(unsigned int sock,char* szbuf,int nlen);


