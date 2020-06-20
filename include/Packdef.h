
#define SERVERIP "192.168.1.103"

#define BEGIN_PROTOCOL_MAP ProtocolMap m_ProtocolMapEntries[] = \
{

#define END_PROTOCOL_MAP {0,0} \
};

#define PM(X,Y) {X,Y},

#define _DEF_PROTOCOL_BASE 10
//注册
#define _DEF_PROTOCOL_REGISTER_RQ   _DEF_PROTOCOL_BASE+1
#define _DEF_PROTOCOL_REGISTER_RS   _DEF_PROTOCOL_BASE+2

//登录
#define _DEF_PROTOCOL_LOGIN_RQ      _DEF_PROTOCOL_BASE+3
#define _DEF_PROTOCOL_LOGIN_RS      _DEF_PROTOCOL_BASE+4


//接收客户端传送的本地视频内容
#define _DEF_PROTOCOL_VEDIO_RQ      _DEF_PROTOCOL_BASE+5
#define _DEF_PROTOCOL_VEDIO_RS      _DEF_PROTOCOL_BASE+6

//接收客户端传送本地视频大小
#define _DEF_PROTOCOL_VEDIO         _DEF_PROTOCOL_BASE+7

//服务器推送视频协议
#define _DEF_PROTOCOL_WATCKVIDEO_RQ  _DEF_PROTOCOL_BASE+8
#define _DEF_PROTOCOL_WATCKVIDEO_RS  _DEF_PROTOCOL_BASE+9

//服务器推送视频大小 内容协议
#define _DEF_PROTOCOL_UPLOADVIDEO_RQ  _DEF_PROTOCOL_BASE+10
#define _DEF_PROTOCOL_UPLOADVIDEOINFO _DEF_PROTOCOL_BASE+11

//点赞协议
#define _DEF_PROTOCOL_ADDLOVENUM      _DEF_PROTOCOL_BASE +12

//下载协议
#define _DEF_PROTOCOL_DOWNLOADINFO         _DEF_PROTOCOL_BASE+13   //下载协议
#define _DEF_PROTOCOL_DOWNLOADSIZE         _DEF_PROTOCOL_BASE+14   //下载文件大小
#define _DEF_PROTOCOL_DOWNLOADCONTENT      _DEF_PROTOCOL_BASE+15   //下载内容协议
#define _DEF_PROTOCOL_CONTENT              _DEF_PROTOCOL_BASE+16   //服务器要发得内容


//临界值
#define register_fail 0
#define register_success 1

#define login_fail 0
#define login_success 1

#define DEF_SIZE 45
#define DEF_CONTENT 10000

typedef char PackType;
typedef struct STRU_REGISTER_RQ
{
	PackType    m_nType;
	//	long long   m_userid;
	int         m_userid;
	//	char        m_userid[DEF_SIZE];
	char        m_szName[DEF_SIZE];
	char        m_szPassword[DEF_SIZE];

}STRU_LOGIN_RQ;

typedef struct STRU_REGISTER_RS
{
	PackType   m_nType;
	char       m_szResult;

}STRU_LOGIN_RS;

//接收客户端上传过来的视频结构体
typedef struct STRU_VEDIOINFO_RQ
{
	PackType   m_nType;
	int        u_id;
	char       m_szContent[DEF_CONTENT];
	char       m_szFilename[DEF_CONTENT];
	char       m_szTitle[DEF_CONTENT];
	int        m_nLove;
	char       m_szType[DEF_CONTENT];
	int        m_nVedioLen;
	int        m_bflag;

}STRU_VEDIOINFO;
struct  STRU_WATCHVIDEO_RQ
{
	PackType    m_nType;
	int   m_userid;
	int         m_n;     //标记变量  0代表播放最初的 1代表上一个 2代表播放下一个 
};


//推送视频结构体
typedef struct  STRU_UPLOAD_RQ
{
	PackType  m_nType;
	int       m_VideoSize;
	char      m_VideoName[DEF_CONTENT];
	char      m_VideoContent[DEF_CONTENT];
	char      m_VideoTitle[DEF_CONTENT];
	char      m_VideoType[DEF_CONTENT];
	int       m_VideoLove;
	int       m_VideoId;
	int       m_VideoCount;
}STRU_UPLOADINFO_RQ;

//点赞数结构体
struct STRU_LOVENUM
{
	PackType   m_nType;
	int        m_videoId;
	int        m_videoLove;
};

//下载结构体
/*struct STRU_DOWNLOAD
{
	PackType   m_nType;
	FILE       *m_pFile;
	int        u_id;     //下载用户id
	int        m_vid;    //要下载视频id
	int        m_nSize;   //文件总大小
	int        m_nPos;    //当前下载位置
	int        m_bflag1;   //下载视频标识
	int        m_bflag2;   //每块接受完标识
	char       m_szpath[1000];  //路径
		 
};*/ 

struct STRU_LoadRq
{
	PackType   m_nType;
	int        m_vid;
	int        m_Flag;        //是否连续下载标记
	int        m_loadPos;       //下载偏移量
	int        m_uid;
};

//文件大小
struct STRU_FILESIZE
{
	PackType   m_nType;
	int        m_nFileSize;
	char       m_nFileName[1000];
};

//客户端请求要发文件内容
struct STRU_FILECONTENT
{
	PackType   m_nType;
	int        m_uid;
};
//内容结构体
struct STRU_CONTENT
{
	PackType  m_nType;
	char      m_nContent[100000];
	int       m_nSize;
};

