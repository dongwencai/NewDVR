#ifndef _JVNSDKDEF_H
#define _JVNSDKDEF_H

#ifndef WIN32
#ifdef __cplusplus
	#define JVSERVER_API extern "C"
#else
	#define JVSERVER_API
#endif
	#include <sys/socket.h>
 
	typedef struct _GUID 
	{ 
		unsigned long   Data1; 
		unsigned short  Data2; 
		unsigned short  Data3; 
		unsigned char   Data4[8]; 
	}GUID;

    #define JVN_BYTE unsigned char
    typedef struct sockaddr SOCKADDR;
#else
	#define JVSERVER_API extern "C" __declspec(dllexport)
	#include <winsock2.h>
#endif

/*实时监控数据类型*/
#define JVN_DATA_I           0x01//视频I帧
#define JVN_DATA_B           0x02//视频B帧
#define JVN_DATA_P           0x03//视频P帧
#define JVN_DATA_A           0x04//音频
#define JVN_DATA_S           0x05//帧尺寸
#define JVN_DATA_OK          0x06//视频帧收到确认
#define JVN_DATA_DANDP       0x07//下载或回放收到确认
#define JVN_DATA_O           0x08//其他自定义数据
#define JVN_DATA_SKIP        0x09//视频S帧
/*请求类型*/
#define JVN_REQ_CHECK        0x10//请求录像检索
#define JVN_REQ_DOWNLOAD     0x20//请求录像下载
#define JVN_REQ_PLAY         0x30//请求远程回放
#define JVN_REQ_CHAT         0x40//请求语音聊天
#define JVN_REQ_TEXT         0x50//请求文本聊天
#define JVN_REQ_CHECKPASS    0x71//请求身份验证
#define JVN_REQ_RECHECK      0x13//预验证
#define JVN_RSP_OLD          0x15//旧版主控回复
#define JVN_REQ_MOTYPE		 0x72//请求身份验证 注：此值与下面一类型定义值相同，本应避免，暂保持这样
/*请求返回结果类型*/
#define JVN_RSP_CHECKDATA    0x11//检索结果
#define JVN_RSP_CHECKOVER    0x12//检索完成
#define JVN_RSP_DOWNLOADDATA 0x21//下载数据
#define JVN_RSP_DOWNLOADOVER 0x22//下载数据完成
#define JVN_RSP_DOWNLOADE    0x23//下载数据失败
#define JVN_RSP_PLAYDATA     0x31//回放数据
#define JVN_RSP_PLAYOVER     0x32//回放完成
#define JVN_RSP_PLAYE        0x39//回放失败
#define JVN_RSP_CHATDATA     0x41//语音数据
#define JVN_RSP_CHATACCEPT   0x42//同意语音请求
#define JVN_RSP_TEXTDATA     0x51//文本数据
#define JVN_RSP_TEXTACCEPT   0x52//同意文本请求
#define JVN_RSP_CHECKPASST   0x72//身份验证成功
#define JVN_RSP_CHECKPASSF   0x73//身份验证失败
#define JVN_RSP_NOSERVER     0x74//无该通道服务
#define JVN_RSP_INVALIDTYPE  0x7A//连接类型无效
#define JVN_RSP_OVERLIMIT    0x7B//连接超过主控允许的最大数目
#define JVN_RSP_DLTIMEOUT    0x76//下载超时
#define JVN_RSP_PLTIMEOUT    0x77//回放超时

/*命令类型*/
#define JVN_CMD_DOWNLOADSTOP 0x24//停止下载数据
#define JVN_CMD_PLAYUP       0x33//快进
#define JVN_CMD_PLAYDOWN     0x34//慢放
#define JVN_CMD_PLAYDEF      0x35//原速播放
#define JVN_CMD_PLAYSTOP     0x36//停止播放
#define JVN_CMD_PLAYPAUSE    0x37//暂停播放
#define JVN_CMD_PLAYGOON     0x38//继续播放
#define JVN_CMD_CHATSTOP     0x43//停止语音聊天
#define JVN_CMD_PLAYSEEK     0x44//播放定位		按帧定位 需要参数 帧数(1~最大帧)
#define JVN_CMD_TEXTSTOP     0x53//停止文本聊天
#define JVN_CMD_YTCTRL       0x60//云台控制
#define JVN_CMD_VIDEO        0x70//实时监控
#define JVN_CMD_VIDEOPAUSE   0x75//暂停实时监控
#define JVN_CMD_TRYTOUCH     0x78//打洞包
#define JVN_CMD_FRAMETIME    0x79//帧发送时间间隔(单位ms)
#define JVN_CMD_DISCONN      0x80//断开连接
#define JVN_RSP_DISCONN      0x7C//断开连接确认

/*与云视通服务器的交互消息*/
#define JVN_CMD_TOUCH        0x81//探测包
#define JVN_REQ_ACTIVEYSTNO  0x82//主控请求激活YST号码
#define JVN_RSP_YSTNO        0x82//服务器返回YST号码
#define JVN_REQ_ONLINE       0x83//主控请求上线
#define JVN_RSP_ONLINE       0x84//服务器返回上线令牌
#define JVN_CMD_ONLINE       0x84//主控地址更新
#define JVN_CMD_OFFLINE      0x85//主控下线
#define JVN_CMD_KEEP         0x86//主控保活
#define JVN_REQ_CONNA        0x87//分控请求主控地址
#define JVN_RSP_CONNA        0x87//服务器向分控返回主控地址
#define JVN_CMD_CONNB        0x87//服务器命令主控向分控穿透
#define JVN_RSP_CONNAF       0x88//服务器向分控返回 主控未上线
#define JVN_CMD_RELOGIN		 0x89//通知主控重新登陆
#define JVN_CMD_CLEAR		 0x8A//通知主控下线并清除网络信息包括云视通号码
#define JVN_CMD_REGCARD		 0x8B//主控注册板卡信息到服务器

#define JVN_CMD_ONLINES2     0x8C//服务器命令主控向转发服务器上线/主控向转发服务器上线
#define JVN_CMD_CONNS2       0x8D//服务器命令分控向转发服务器发起连接
#define JVN_REQ_S2           0x8E//分控向服务器请求转发
#define JVN_TDATA_CONN       0x8F//分控向转发服务器发起连接
#define JVN_TDATA_NORMAL     0x90//分控/主控向转发服务器发送普通数据

#define JVN_CMD_CARDCHECK    0x91//板卡验证
#define JVN_CMD_ONLINEEX     0x92//主控地址更新扩展
#define JVN_CMD_TCPONLINES2  0x93//服务器命令主控TCP向转发服务器上线
#define JVN_CMD_CHANNELCOUNT 0x97//分控查询主控所具有的通道数目
#define JVN_CMD_ONLINE1EX    0x9C//主控UDP1上线扩展(新上线)
#define JVN_CMD_YSTCHECK     0xAC//查询及返回某号码是否在线以及号码主控SDK版本

/*客户版流媒体服务器相关*/
#define JVN_REQ_CONNSTREAM_SVR    0xD0//请求连接流媒体服务器主控
#define JVN_REQ_CONNSTREAM_CLT    0xD1//请求连接流媒体服务器分控
#define JVN_RSP_CONNSTREAM_SVR    0xD2//回复
#define JVN_RSP_CONNSTREAM_CLT    0xD3
#define JVN_NOTIFY_ID			  0xD4
#define JVN_RSP_ID				  0xD5
#define JVN_CMD_CONNSSERVER		  0xD6
#define JVN_RSP_NEWCLIENT         0xD9

/*局域网设备搜索*/
#define JVN_REQ_LANSERCH  0x01//局域网设备搜索命令
#define JVN_CMD_LANSALL   1//局域网搜索所有中维设备
#define JVN_CMD_LANSYST   2//局域网搜索指定云视通号码的设备
#define JVN_CMD_LANSTYPE  3//局域网搜索指定卡系的设备
#define JVN_CMD_LANSNAME  4//局域网搜索指定别名的设备
#define JVN_RSP_LANSERCH  0x02//局域网设备搜索响应命令

#define JVN_ALLSERVER     0//所有服务
#define JVN_ONLYNET       1//只局域网服务

#define JVN_NOTURN        0//云视通方式时禁用转发
#define JVN_TRYTURN       1//云视通方式时启用转发
#define JVN_ONLYTURN      2//云视通方式时仅用转发

#define JVN_LANGUAGE_ENGLISH  1
#define JVN_LANGUAGE_CHINESE  2

#define TYPE_PC_UDP      1//连接类型 UDP 支持UDP收发完整数据
#define TYPE_PC_TCP      2//连接类型 TCP 支持TCP收发完整数据
#define TYPE_MO_TCP      3//连接类型 TCP 支持TCP收发简单数据,普通视频帧等不再发送，只能采用专用接口收发数据(适用于手机监控)
#define TYPE_MO_UDP      4//连接类型 TCP 支持TCP收发简单数据,普通视频帧等不再发送，只能采用专用接口收发数据(适用于手机监控)

/*云台控制类型*/
#define JVN_YTCTRL_U      1//上
#define JVN_YTCTRL_D      2//下
#define JVN_YTCTRL_L      3//左
#define JVN_YTCTRL_R      4//右
#define JVN_YTCTRL_A      5//自动
#define JVN_YTCTRL_GQD    6//光圈大
#define JVN_YTCTRL_GQX    7//光圈小
#define JVN_YTCTRL_BJD    8//变焦大
#define JVN_YTCTRL_BJX    9//变焦小
#define JVN_YTCTRL_BBD    10//变倍大
#define JVN_YTCTRL_BBX    11//变倍小

#define JVN_YTCTRL_UT     21//上停止
#define JVN_YTCTRL_DT     22//下停止
#define JVN_YTCTRL_LT     23//左停止
#define JVN_YTCTRL_RT     24//右停止
#define JVN_YTCTRL_AT     25//自动停止
#define JVN_YTCTRL_GQDT   26//光圈大停止
#define JVN_YTCTRL_GQXT   27//光圈小停止
#define JVN_YTCTRL_BJDT   28//变焦大停止
#define JVN_YTCTRL_BJXT   29//变焦小停止
#define JVN_YTCTRL_BBDT   30//变倍大停止
#define JVN_YTCTRL_BBXT   31//变倍小停止

#define JVN_YTCTRL_RECSTART  41//远程录像开始
#define JVN_YTCTRL_RECSTOP	 42//远程录像开始

/*主控回调函数*/
typedef bool (*FUNC_SCHECKPASS_CALLBACK)(int nLocalChannel, char chClientIP[16], int nClientPort, char *pName, char *pWord);
typedef void (*FUNC_SCONNECT_CALLBACK)(int nLocalChannel, int nClientID, unsigned char uchType, char chClientIP[16], int nClientPort, char *pName, char *pWord);
typedef void (*FUNC_SONLINE_CALLBACK)(int nLocalChannel, unsigned char uchType);
typedef void (*FUNC_SCHECKFILE_CALLBACK)(int nLocalChannel, int nClientID, char chClientIP[16], int nClientPort, char chStartTime[14], char chEndTime[14], JVN_BYTE *pBuffer, int *nSize);
typedef void (*FUNC_SCHAT_CALLBACK)(int nLocalChannel, int nClientID, unsigned char uchType, char chClientIP[16], int nClientPort, JVN_BYTE *pBuffer, int nSize);
typedef void (*FUNC_STEXT_CALLBACK)(int nLocalChannel, int nClientID, unsigned char uchType, char chClientIP[16], int nClientPort, JVN_BYTE *pBuffer, int nSize);
typedef void (*FUNC_SYTCTRL_CALLBACK)(int nLocalChannel, int nClientID, int nType, char chClientIP[16], int nClientPort);
/*分控回调函数*/
typedef void (*FUNC_CCONNECT_CALLBACK)(int nLocalChannel, unsigned char uchType, char *pMsg);
typedef void (*FUNC_CNORMALDATA_CALLBACK)(int nLocalChannel, unsigned char uchType, JVN_BYTE *pBuffer, int nSize, int nWidth, int nHeight);
typedef void (*FUNC_CCHECKRESULT_CALLBACK)(int nLocalChannel,JVN_BYTE *pBuffer, int nSize);
typedef void (*FUNC_CCHATDATA_CALLBACK)(int nLocalChannel, unsigned char uchType, JVN_BYTE *pBuffer, int nSize);
typedef void (*FUNC_CTEXTDATA_CALLBACK)(int nLocalChannel, unsigned char uchType, JVN_BYTE *pBuffer, int nSize);
typedef void (*FUNC_CDOWNLOAD_CALLBACK)(int nLocalChannel, unsigned char uchType, JVN_BYTE *pBuffer, int nSize, int nFileLen);
typedef void (*FUNC_CPLAYDATA_CALLBACK)(int nLocalChannel, unsigned char uchType, JVN_BYTE *pBuffer, int nSize, int nWidth, int nHeight, int nTotalFrame);
typedef void (*FUNC_CLANSDATA_CALLBACK)(int nYSTNO, int nCardType, int nChannelCount, char chClientIP[16], int nClientPort, BOOL bTimoOut);
/************主控回调函数参数类型***************/
/*FUNC_SCONNECT_CALLBACK*/
#define JVN_SCONNECTTYPE_PCCONNOK  0X01//有PC分控连接成功
#define JVN_SCONNECTTYPE_DISCONNOK 0X02//有分控断开连接成功
#define JVN_SCONNECTTYPE_DISCONNE  0X03//连接异常断开
#define JVN_SCONNECTTYPE_MOCONNOK  0X04//有移动设备分控连接成功
/*FUNC_SONLINE_CALLBACK*/
#define JVN_SONLINETYPE_ONLINE     0x01//上线
#define JVN_SONLINETYPE_OFFLINE    0x02//下线
#define JVN_SONLINETYPE_CLEAR      0x03//YST号码无效，应清空重新申请

/************分控回调函数参数类型***************/
/*FUNC_CCONNECT_CALLBACK*/
#define JVN_CCONNECTTYPE_CONNOK    0X01//连接成功
#define JVN_CCONNECTTYPE_DISOK     0X02//断开连接成功
#define JVN_CCONNECTTYPE_RECONN    0X03//不必重复连接
#define JVN_CCONNECTTYPE_CONNERR   0X04//连接失败
#define JVN_CCONNECTTYPE_NOCONN    0X05//没连接
#define JVN_CCONNECTTYPE_DISCONNE  0X06//连接异常断开
#define JVN_CCONNECTTYPE_SSTOP     0X07//服务停止，连接断开
#define JVN_CCONNECTTYPE_DISF      0x08//断开连接失败


/*返回值*/
#define JVN_RETURNOK    0//成功
#define JVN_PARAERROR   1//参数错误
#define JVN_RETURNERROR 2//失败


/*激活号码时调用者传入结构*/
typedef struct 
{
	char chGroup[4];//分组号，形如"A" "AAAA"
	int nCardType;  //卡系
	int	nDate;      //出厂日期 形如 20091011
	int	nSerial;    //出厂序列号
	GUID guid;      //唯一GUID MAPIGUID.H
}STGetNum;
/*初始化时调用者传入的结构*/
typedef struct
{
	char chGroup[4];//分组号，形如"A" "AAAA"
	int nCardType; //卡系
	int	nDate;     //出厂日期
	int	nSerial;   //出厂序列号
	GUID gLoginKey;//板卡GUID
	int  nYstNum;  //云视通号码
}STOnline;



/*数据包:激活YST号码*/
typedef struct
{
	int nCardType;  //卡系
	int	nDate;      //出厂日期 形如 20091011
	int	nSerial;    //出厂序列号
	GUID guid;      //唯一GUID MAPIGUID.H
}Pak_GetNum;

/*数据包:上线*/
typedef struct
{
	int nCardType; //卡系
	int	nDate;     //出厂日期
	int	nSerial;   //出厂序列号
	GUID gLoginKey;//板卡GUID
	int  nYstNum;  //云视通号码
}Pak_Online1; 

typedef struct
{
	int		nType;//板卡类型
	int		nDate; //出厂日期
	int		nSerial; //出厂序列号
	GUID	gLoginKey; //登陆令牌
	int     nYstNum;   //云视通号码
	char    chGroup[4];//编组号 暂不使用
	int     nNSDKVer;//主控网络SDK版本(协议版本)
	int     nChannelCount; //主控通道总数目
}Pak_Online1Ex; //YST_AS_ONLINE1  数据结构

/*数据包:上线应答*/
typedef struct
{
	int	nIndex;     //顺序号
	GUID gLoginKey; //登陆令牌
}Pak_LoginKey; 

/*数据包: 地址更新*/
typedef struct
{
	int nYstNum;       //云视通号码
	int	nIndex;        //顺序号
	GUID gLoginKey;    //登陆令牌
	SOCKADDR InAddress;//内网地址
	int nVer;          //主控的版本
}Pak_Online2;

/*数据包: 地址更新*/
typedef struct
{
	int nYstNum;       //云视通号码
	int	nIndex;        //顺序号
	GUID gLoginKey;    //登陆令牌
	SOCKADDR InAddress;//内网地址
	int nVer;          //主控的版本
	int nTCPSerPort;   //TCP服务端口
}Pak_Online2Ex;

/*数据包: 下线*/
typedef struct
{
	int	nYstNum;
	int	nIndex;
	GUID gLoginKey;//登陆令牌
}Pak_Logout;

typedef struct
{
	unsigned int nTypeLens; //YST_AS_REGCARD	
	//净载数据
	int nSize;
	char *pacData;
}SOCKET_DATA_TRAN;

//接口函数
JVSERVER_API bool JVN_InitSDK(int nYSTPort, int nLocPort, int nSPort, int nVersion);
JVSERVER_API bool JVN_InitSDKEx(int nYSTPort, int nLocPort, int nSPort, int nVersion, long lRSBuffer);
JVSERVER_API void JVN_ReleaseSDK();
JVSERVER_API void JVN_RegisterCallBack(FUNC_SCHECKPASS_CALLBACK CheckPassCallBack,
									  FUNC_SONLINE_CALLBACK OnlineCallBack,
									  FUNC_SCONNECT_CALLBACK ConnectCallBack,
									  FUNC_SCHECKFILE_CALLBACK CheckFileCallBack,
									  FUNC_SYTCTRL_CALLBACK YTCtrlCallBack,
									  FUNC_SCHAT_CALLBACK ChatCallBack,
									  FUNC_STEXT_CALLBACK TextCallBack);
JVSERVER_API bool JVN_ActiveYSTNO(char *pchPackGetNum, int nLen, int *nYSTNO);
JVSERVER_API void JVN_InitYST(char *pchPackOnline, int nLen);
JVSERVER_API bool JVN_StartChannel(int nChannel, int nType, long lBufLen);
JVSERVER_API void JVN_StopChannel(int nChannel);
JVSERVER_API void JVN_SendData(int nChannel,unsigned char uchType,JVN_BYTE *pBuffer,int nSize,int nWidth,int nHeight);
JVSERVER_API void JVN_SendDataTo(int nClientID,unsigned char uchType,JVN_BYTE *pBuffer,int nSize,int nWidth,int nHeight);
JVSERVER_API void JVN_SendChatData(int nChannel,int nClientID,unsigned char uchType,JVN_BYTE *pBuffer,int nSize);
JVSERVER_API void JVN_EnableLog(bool bEnable);
JVSERVER_API void JVN_SetLanguage(int nLgType);
JVSERVER_API bool JVN_RegCard(char chGroup[4], JVN_BYTE *pBuffer,int nSize);
JVSERVER_API bool JVN_SendAndRecvS(char chGroup[4], JVN_BYTE *pBuffer,int nSize, JVN_BYTE *pRecvBuffer,int *nRecvLen,int nTimeOut);
JVSERVER_API bool JVN_StartWebServer(char chHomeDir[256], char chDefIndex[256], char chLocalIP[30], int nPort);
JVSERVER_API void JVN_StopWebServer();
JVSERVER_API bool JVN_StartTCPServer(int nPort);
JVSERVER_API void JVN_StopTCPServer();
JVSERVER_API int JVN_GetLanguage();
JVSERVER_API void JVN_SetClientLimit(int nChannel, int nMax);
JVSERVER_API int JVN_GetClientLimit(int nChannel);
JVSERVER_API void JVN_SendMOTCPData(int nChannel,unsigned char uchType,JVN_BYTE *pBuffer,int nSize);
JVSERVER_API void JVN_SendMOTCPDataDirect(int nChannel,unsigned char uchType,JVN_BYTE *pBuffer, int nOffset, int nSize);
JVSERVER_API void JVN_SendDataDirect(int nChannel, JVN_BYTE uchType, JVN_BYTE *pBuffer, int nOffset, int nSize);


/****************************************************************************
*名称  : JVN_StartLANSerchServer
*功能  : 开启局域网搜索服务
*参数  : [IN] nChannelCount 当前设备总的通道数
         [IN] nPort         服务端口号(<=0时为默认9103,建议使用默认值与分控端统一)
*返回值: 无
*其他  : 无
*****************************************************************************/
#ifndef WIN32
	JVSERVER_API bool JVN_StartLANSerchServer(int nChannelCount, int nPort);
#else
	JVSERVER_API bool __stdcall JVN_StartLANSerchServer(int nChannelCount, int nPort);
#endif

/****************************************************************************
*名称  : JVN_StopLANSerchServer
*功能  : 停止局域网搜索服务
*参数  : 无
*返回值: 无
*其他  : 无
*****************************************************************************/
#ifndef WIN32
	JVSERVER_API void JVN_StopLANSerchServer();
#else
	JVSERVER_API void __stdcall JVN_StopLANSerchServer();
#endif


/****************************************************************************
*名称  : JVN_SetLocalFilePath
*功能  : 自定义本地文件存储路径，包括日志，生成的其他关键文件等 
*参数  : [IN] chLocalPath  路径 形如："C:\\jovision"  其中jovision是文件夹
*返回值: 无
*其他  : 参数使用内存拷贝时请注意初始化，字符串需以'\0'结束
*****************************************************************************/
#ifndef WIN32
	JVSERVER_API bool JVN_SetLocalFilePath(char chLocalPath[256]);
#else
	JVSERVER_API bool __stdcall	JVN_SetLocalFilePath(char chLocalPath[256]);
#endif

#endif
