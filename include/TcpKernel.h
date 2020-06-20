#include<TCPNet.h>
#include<Packdef.h>
#include<strings.h>
#include<stdio.h>
#include<MySql.h>

//用户信息
struct Author_Info
{
	int sock; //用户socket
	int m_id; //视频id
	FILE *pFile; //文件描述符
	int offset; //偏移量
	int nSize; //文件总大小
	char pInfoFileName[100];

};
//用户链表
List *list;
//用户下载链表
List *downlist;

//用户下载结构体成员
typedef struct UserDownload_Info
{
	int        u_id;     //下载用户id
	int        m_vid;    //要下载视频id
	FILE*      m_pFile;
	int        m_nSize;   //文件总大小
	int        m_nPos;    //当前下载位置
	int       m_bflag1;   //下载视频标识
	int       m_bflag2;   //每块接受完标识
	char       m_szpath[1000];  //路径
}STRU_DOWNINFO;

 
typedef void (*PFUN)(unsigned int,char*);
typedef struct 
{
	PackType m_nType;
	PFUN     m_npfun;

}ProtocolMap;


char szpath[1024];

int   Kernel_open();
void  Kernel_close();
int   Kernel_DealData();
int   DealData(unsigned int clientfd,char* szbuf);

void RegisterRq(unsigned int clientfd,char*szbuf);
void LoginRq(unsigned int clientfd,char* szbuf);
void SaveVideoInfo(unsigned int clientfd,char* szbuf);
void SaveVideoContent(unsigned int clientfd,char* szbuf);
void WatchVideo(unsigned int clientfd,char* szbuf); //用户想要观看视频请求
void AddLoveNum(unsigned int clientfd,char* szbuf);
void DownLoadInfo(unsigned int clientfd,char* szbuf);  //下载函数
void SendloadContent(unsigned int clientfd,char* szbuf);//发送下载内容
