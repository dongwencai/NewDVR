#ifndef JVSERVER_H
#define JVSERVER_H
#include "JVNSDKDef-V2.h"
#ifndef WIN32
	//#include "JVNSDKDef.h"
	#ifdef __cplusplus
		#define JVSERVER_API extern "C"
	#else
		#define JVSERVER_API
	#endif
#else
	#define JVSERVER_API extern "C" __declspec(dllexport)
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                     //
//                                      主控端接口                                                     //
//                                                                                                     //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/***************************************接口清单********************************************************
JVN_InitSDK -----------------01 初始化SDK资源
JVN_ReleaseSDK---------------02 释放SDK资源
JVN_RegisterCallBack --------03 设置主控端回调函数
JVN_ActiveYSTNO -------------04 激活云视通号码
JVN_InitYST -----------------05 初始化云视通服务
JVN_StartChannel ------------06 开启某通道网络服务
JVN_StopChannel -------------07 停止某通道所有服务
JVN_SendData ----------------08 发送数据(通道群发)
JVN_SendDataTo --------------09 发送数据(定向发送)
JVN_SendChatData ------------10 发送聊天信息(语音聊天和文本聊天)
JVN_EnableLog ---------------11 设置写出错日志是否有效
JVN_RegCard -----------------12 注册产品
JVN_SetLanguage -------------13 设置日志/提示信息语言(英文/中文)
JVN_GetLanguage -------------14 获取日志/提示信息语言(英文/中文)
JVN_SendAndRecvS ------------15 与最快服务器交互
JVN_StartWebServer ----------16 开启web服务
JVN_StartSelfWebServer ------17 开启自定义web服务(OEM)
JVN_StopWebServer -----------18 停止web服务
JVN_SetClientLimit ----------19 设置分控数目上限
JVN_GetClientLimit ----------20 获取分控数目上限设置值
JVN_EnablePCTCPServer -------21 开启关闭PC客户TCP服务(保留暂未使用)
JVN_EnableMOServer ----------22 开启关闭某通道的手机服务
JVN_SendMOData --------------23 发送手机数据，对TYPE_MO_TCP/TYPE_MO_UDP连接有效
JVN_Command -----------------24 运行特定指令，要求SDK执行特定操作
JVN_StartLANSerchServer -----25 开启局域网搜索服务
JVN_StopLANSerchServer ------26 停止局域网搜索服务
JVN_SetLocalNetCard ---------27 设置哪一张网卡(IPC)
JVN_SetDeviceName -----------28 设置本地设备别名
JVN_SetDomainName -----------29 设置新的域名，系统将从其获取服务器列表
JVN_SetLocalFilePath --------30 自定义本地文件存储路径，包括日志，生成的其他关键文件等
JVN_StartStreamService ------31 开启流媒体服务
JVN_StopStreamService -------32 停止流媒体服务
JVN_GetStreamServiceStatus --33 查询流媒体服务状态
JVN_StartBroadcastServer ----34 开启自定义广播服务(IPC)
JVN_StopBroadcastServer -----35 关闭自定义广播服务(IPC)
JVN_BroadcastRSP ------------36 发送自定义广播应答(IPC)
JVN_SendPlay-----------------37 向指定目标发送远程回放数据,暂用于MP4文件远程回放
JVN_EnableLANToolServer------38 开启关闭局域网生产工具服务
JVN_RegDownLoadFileName------39 注册回调函数，用于调用者特殊处理远程下载文件名
JVN_SetIPCWANMode------------40 启用IPC外网特殊处理模式
JVN_SetDeviceInfo------------41 设置设备信息
JVN_GetDeviceInfo------------42 获取设备信息
JVN_SetLSPrivateInfo---------43 设置本地自定义信息，用于设备搜索
*******************************************************************************************************/                                                                                                     //


/****************************************************************************
*名称  : JVN_InitSDK
*功能  : 初始化SDK资源，必须被第一个调用
*参数  : [IN] nYSTPort    用于与云视通服务通信的端口,0时默认9100
         [IN] nLocPort    用于提供主控服务通信的端口,0时默认9101
		 [IN] nSPort      用于与云视通服务器交互(激活号码，注册等)的端口,0时默认9102
		 [IN] nVersion    主控版本号，用于云视通服务器验证
		 [IN] lRSBuffer   主控端用于收发单帧数据的最小缓存，为0时默认150K，
		                  IPC高清数据单帧可能很大，可根据最大帧确定该值，若调大则对应分控端也需要相应调大.
						  该参数需求来自IPC；
		 [IN] bLANSpeedup 是否对内网连接优化提速(IPC局域网大码流传输特殊处理,其他普通码流产品建议置为FALSE)
		 [IN] stDeviceType 产品类型 详见JVNSDKDef.h 中 STDeviceType
		                   普通板卡样例：stDeviceType.nType = 1;stDeviceType.nMemLimit = 1;
						   普通IPC样例： stDeviceType.nType = 3;stDeviceType.nMemLimit = 1;
						   内存严重不足DVR样例：stDeviceType.nType = 2;stDeviceType.nMemLimit = 3;
		 [IN] nDSize       请填入sizeof(STDeviceType),用于校验传入结构合法性
*返回值: TRUE     成功
         FALSE    失败
*其他  : 主控端长期占用两个端口，一个用于与云视通服务器通信，一个用于与分控通信
*****************************************************************************/
#ifndef WIN32
	JVSERVER_API int JVN_InitSDK(int nYSTPort, int nLocPort, int nSPort, int nVersion, long lRSBuffer, int bLANSpeedup, STDeviceType stDeviceType, int nDSize);
#else
	JVSERVER_API bool __stdcall	JVN_InitSDK(int nYSTPort, int nLocPort, int nSPort, int nVersion, long lRSBuffer, BOOL bLANSpeedup, STDeviceType stDeviceType, int nDSize);
#endif

/****************************************************************************
*名称  : JVN_ReleaseSDK
*功能  : 释放SDK资源，必须最后被调用 
*参数  : 无
*返回值: 无
*其他  : 无
*****************************************************************************/
#ifndef WIN32
	JVSERVER_API void JVN_ReleaseSDK();
#else
	JVSERVER_API void __stdcall	JVN_ReleaseSDK();
#endif

/****************************************************************************
*名称  : JVN_RegisterSCallBack
*功能  : 设置主控端回调函数 
*参数  : [IN] ConnectCallBack   分控连接状况回调函数
         ...
*返回值: 无
*其他  : 主控端回调函数包括：
             分控身份验证函数            (身份验证)
             与云视通服务器通信状态函数；(上线状态)
		     与分控端通信状态函数；      (连接状态)
		     录像检索函数；              (录像检索请求)
			 远程云台控制函数；          (远程云台控制)
			 语音聊天/文本聊天函数       (远程语音和文本聊天)
			 回放控制       (目前的回放控制mp4文件)
*****************************************************************************/
#ifndef WIN32
	JVSERVER_API void JVN_RegisterCallBack(FUNC_SCHECKPASS_CALLBACK CheckPassCallBack,
											FUNC_SONLINE_CALLBACK OnlineCallBack,
											FUNC_SCONNECT_CALLBACK ConnectCallBack,
											FUNC_SCHECKFILE_CALLBACK CheckFileCallBack,
											FUNC_SYTCTRL_CALLBACK YTCtrlCallBack,
											FUNC_SCHAT_CALLBACK ChatCallBack,
											FUNC_STEXT_CALLBACK TextCallBack,
											FUNC_SFPLAYCTRL_CALLBACK FPlayCtrlCallBack);
#else
	JVSERVER_API void __stdcall	JVN_RegisterCallBack(FUNC_SCHECKPASS_CALLBACK CheckPassCallBack,
												  FUNC_SONLINE_CALLBACK OnlineCallBack,
												  FUNC_SCONNECT_CALLBACK ConnectCallBack,
												  FUNC_SCHECKFILE_CALLBACK CheckFileCallBack,
												  FUNC_SYTCTRL_CALLBACK YTCtrlCallBack,
												  FUNC_SCHAT_CALLBACK ChatCallBack,
												  FUNC_STEXT_CALLBACK TextCallBack,
												  FUNC_SFPLAYCTRL_CALLBACK FPlayCtrlCallBack);
#endif

/****************************************************************************
*名称  : JVN_ActiveYSTNO
*功能  : 激活云视通号码
*参数  : [IN]  pchPackGetNum  卡信息(STGetNum结构)
         [IN]  nLen           信息长度(sizeof(STGetNum))
         [OUT] nYSTNO         成功激活的云视通号码
*返回值: TRUE  成功
         FALSE 失败
*其他  : 云视通服务器地址在内部获取；
         函数独立使用，只返回激活的YST号码，程序内不做任何记录。
*****************************************************************************/
#ifndef WIN32
	JVSERVER_API int JVN_ActiveYSTNO(char *pchPackGetNum, int nLen, int *nYSTNO);
#else
	JVSERVER_API bool __stdcall	JVN_ActiveYSTNO(char *pchPackGetNum, int nLen, int &nYSTNO);
#endif

/****************************************************************************
*名称  : JVN_InitYST
*功能  : 初始化云视通服务
*参数  : [IN] 云视通号码等信息(STOnline结构)
         [IN] 信息长度
*返回值: 无
*其他  : 该函数需在启动通道云视通服务前调用，否则通道云视通服务将启动失败；
         
		 该函数只需调用一次，即，若所有通道中只要有需要启动云视通服务的，
		 在启动服务前调用一次该接口即可；
         
		 该函数记录云视通号码及服务，在上线时使用这些参数。
*****************************************************************************/
#ifndef WIN32
	JVSERVER_API void JVN_InitYST(char *pchPackOnline, int nLen);
#else
	JVSERVER_API void __stdcall	JVN_InitYST(char *pchPackOnline, int nLen);
#endif

/****************************************************************************
*名称  : JVN_StartChannel
*功能  : 开启某通道网络服务
*参数  : [IN] nChannel  服务通道号 >=1  
                        特别:nChannel=-1时特殊通道，可用于避开视频通道，传输普通数据

         [IN] nType     服务启动类型，0内外网服务都启动；1只启动局域网服务；

		 [IN] bJVP2P    开启中维网络直播，内存不充足或是没有硬盘时该功能不能启用；
		                即为TRUE时为多播方式；为FALSE时为传统方式；
						建议给用户展现时对应的两个选项：常规模式(无延时)和大连结数模式(小量延时)；
						默认为常规模式即可；
						没有特殊需要,可将该功能置为FALSE,不必展现给用户；

		 [IN] lBufLen   通道视频帧缓冲区大小，单位字节, 
		                普通方式时缓存大小指两个帧序列数据量(需>400*1024,应按最大码流计算)；
						JVP2P方式时建议置为>=8M，最小为1024000；
						如果该值设置不足，可能产生每个帧序列末尾的帧丢失造成卡顿的现象；

		 [IN] chBufPath 缓存文件路径，不使用文件缓存时置为""; 

*返回值: TRUE  成功
         FALSE 失败
*其他  : 每个通道的视频帧尺寸不同，因此所需缓冲区也不同，由应用层设置；
         该函数可重复调用，参数有变化时才进行实际操作，重复的调用会被忽略；
		 应用层在更新了某些设置后，可重新调用该接口设置服务。

         若开启了jvp2p，则有两种方式进行缓存：内存方式和文件方式
		 即lbuflen 和 chBufPath必须有一个是有效的，如果同时有效则优先采用内存方式，两者都无效则失败

		 建议：1.内存方式时每个通道建议>=8M内存，效果最佳，内存若充足则建议采用内存方式；
		       2.DVR等内存非常紧张的设备可以安装硬盘并且采用文件存储方式使用jvp2p。
			     DVR内存较充足的设备建议至少每个通道分配>=1M的视频缓冲区，否则不建议使用；
				 DVR内存紧张设备无法使用jvp2p，不必展现给用户，仅默认提供'常规模式'即可；
			   3.每个通道都可以单独开启jvp2p功能，但出于应用层管理的简便以及多通道对整体带宽的竞争，
			     建议所有通道统一开启或关闭该功能；即若使用jvp2p，则所有通道都使用，否则都不用；

         bJVP2P=TRUE时，将以"大连结数和保证流畅"的模式运行，并且主控端只要提供了必要的基础上传带宽，比如2M，
		 就能满足几十上百人同时连接，若配合开通中维VIP转发，将能确保画面流畅。但实时性稍差，
		 远程画面与实际画面可能延时2s-7s；
		 bJVP2P=FALSE时，将以"尽可能无延时"的模式运行，但连接数完全取决于带宽和设备资源，是一种传统传输方式；
*****************************************************************************/
#ifndef WIN32
	JVSERVER_API int JVN_StartChannel(int nChannel, int nType, int bJVP2P, long lBufLen, char chBufPath[256]);
#else
	JVSERVER_API bool __stdcall	JVN_StartChannel(int nChannel, int nType, BOOL bJVP2P, long lBufLen, char chBufPath[MAX_PATH]);
#endif

/****************************************************************************
*名称  : JVN_StopChannel
*功能  : 停止某通道所有服务 
*参数  : [IN] nChannel 服务通道号 >=1
*返回值: 无
*其他  : 停止某个服务也可通过重复调用JVN_StartChannel实现；
         若想停止所有服务，只能通过该接口实现。
*****************************************************************************/
#ifndef WIN32
	JVSERVER_API void JVN_StopChannel(int nChannel);
#else
	JVSERVER_API void __stdcall	JVN_StopChannel(int nChannel);
#endif

/****************************************************************************
*名称  : JVN_SendData
*功能  : 发送数据 
*参数  : [IN] nChannel   服务通道号 >=1
         [IN] uchType    数据类型：视频I;视频P;视频B;视频S;音频;尺寸;自定义类型;帧发送时间间隔
         [IN] pBuffer    待发数据内容,视频/音频/自定义数据时有效
		 [IN] nSize      待发数据长度,视频/音频/自定义数据时有效
		 [IN] nWidth     uchType=JVN_DATA_S时表示帧宽/uchType=JVN_CMD_FRAMETIME时表示帧间隔(单位ms)
		 [IN] nHeight    uchType=JVN_DATA_S时表示帧高/uchType=JVN_CMD_FRAMETIME时表示关键帧周期
*返回值: 无
*其他  : 以通道为单位，向通道连接的所有分控发送数据
*****************************************************************************/
#ifndef WIN32
	JVSERVER_API void JVN_SendData(int nChannel,unsigned char uchType,unsigned char *pBuffer,int nSize,int nWidth,int nHeight);
#else
	JVSERVER_API void __stdcall	JVN_SendData(int nChannel,unsigned char uchType,BYTE *pBuffer,int nSize,int nWidth,int nHeight);
#endif

/****************************************************************************
*名称  : JVN_SendDataTo
*功能  : 发送数据 
*参数  : [IN] nChannel   服务通道号 >=1
         [IN] uchType    数据类型：目前只用于尺寸发送尺寸;断开某连接;自定义类型
         [IN] pBuffer    待发数据内容
		 [IN] nSize      待发数据长度
		 [IN] nWidth     uchType=JVN_DATA_S时表示帧宽/uchType=JVN_CMD_FRAMETIME时表示帧间隔(单位ms)
		 [IN] nHeight    uchType=JVN_DATA_S时表示帧高/uchType=JVN_CMD_FRAMETIME时表示关键帧周期
*返回值: 无
*其他  : 向通道连接的某个具体分控发送数据
*****************************************************************************/
#ifndef WIN32
	JVSERVER_API void JVN_SendDataTo(int nClientID,unsigned char uchType,unsigned char *pBuffer,int nSize,int nWidth,int nHeight);
#else
	JVSERVER_API void __stdcall	JVN_SendDataTo(int nClientID,unsigned char uchType,BYTE *pBuffer,int nSize,int nWidth,int nHeight);
#endif

/****************************************************************************
*名称  : JVN_SendChatData
*功能  : 发送聊天信息(语音聊天和文本聊天)
*参数  : [IN] nChannel   服务通道号 >=1,广播式发送时有效;
         [IN] nClientID  分控ID,向指定分控发送,此时nChannel无效,优先级高于nChannel;
         [IN] uchType      数据类型：语音请求;
		                             文本请求;
		                             同意语音请求;
                                     同意文本请求;
								     语音数据;
								     文本数据;
								     语音关闭;
								     文本关闭;
         [IN] pBuffer    待发数据内容
         [IN] nSize      待发数据长度
*返回值: true   成功
         false  失败
*其他  : 调用者将聊天数据发送给请求语音服务的分控端;
         nChannel和nClientID不能同时<=0,即不能同时无效;
		 nChannel和nClientID同时>0时,nClientID优先级高,此时只向指定分控发送。
*****************************************************************************/
#ifndef WIN32
	JVSERVER_API int JVN_SendChatData(int nChannel,int nClientID,unsigned char uchType,unsigned char *pBuffer,int nSize);
#else
	JVSERVER_API bool __stdcall	JVN_SendChatData(int nChannel,int nClientID,unsigned char uchType,BYTE *pBuffer,int nSize);
#endif

/****************************************************************************
*名称  : JVN_EnableLog
*功能  : 设置写出错日志是否有效 
*参数  : [IN] bEnable  TRUE:出错时写日志；FALSE:不写任何日志
*返回值: 无
*其他  : 无
*****************************************************************************/
#ifndef WIN32
	JVSERVER_API void JVN_EnableLog(int bEnable);
#else
	JVSERVER_API void __stdcall	JVN_EnableLog(bool bEnable);
#endif

/****************************************************************************
*名称  : JVN_SetLanguage
*功能  : 设置日志/提示信息语言(英文/中文) 
*参数  : [IN] nLgType  JVN_LANGUAGE_ENGLISH/JVN_LANGUAGE_CHINESE
*返回值: 无
*其他  : 无
*****************************************************************************/
#ifndef WIN32
	JVSERVER_API void JVN_SetLanguage(int nLgType);
#else
	JVSERVER_API void __stdcall	JVN_SetLanguage(int nLgType);
#endif

/****************************************************************************
*名称  : JVN_GetLanguage
*功能  : 获取日志/提示信息语言(英文/中文) 
*参数  : 无
*返回值: JVN_LANGUAGE_ENGLISH/JVN_LANGUAGE_CHINESE
*其他  : 无
*****************************************************************************/
#ifndef WIN32
	JVSERVER_API int JVN_GetLanguage();
#else
	JVSERVER_API int __stdcall	JVN_GetLanguage();
#endif

/****************************************************************************
*名称  : JVN_SetClientLimit
*功能  : 设置分控数目上限
*参数  : [IN] nChannel  通道号(>0;=0;<0)
         [IN] nMax      数目上限值 <0时表示无限制
		                nChannel<0 时表示分控总数目上限为nMax; 
						nChannel=0 时表示所有通道使用相同单通道分控数目上限为nMax; 
						nChannel>0 时表示单通道分控数目上限为nMax
*返回值: 无
*其他  : 可重复调用,以最后一次设置为准;
         总数上限和单通道上限可同时起作用;
*****************************************************************************/
#ifndef WIN32
	JVSERVER_API void JVN_SetClientLimit(int nChannel, int nMax);
#else
	JVSERVER_API void __stdcall	JVN_SetClientLimit(int nChannel, int nMax);
#endif

/****************************************************************************
*名称  : JVN_GetClientLimit
*功能  : 获取分控数目上限设置值
*参数  : [IN] nChannel  通道号(>0;<0)
		                nChannel<0 时表示获取分控总数目上限; 
						nChannel>0 时表示获取单通道分控数目上限;
*返回值: 数目上限值 <=0表示无限制
*其他  : 无
*****************************************************************************/
#ifndef WIN32
	JVSERVER_API int JVN_GetClientLimit(int nChannel);
#else
	JVSERVER_API int __stdcall	JVN_GetClientLimit(int nChannel);
#endif

/****************************************************************************
*名称  : JVN_RegCard
*功能  : 注册产品
*参数  : [IN] chGroup    分组号，形如"A" "AAAA"
         [IN] pBuffer    待发数据内容(SOCKET_DATA_TRAN结构)
		 [IN] nSize      待发数据总长度
*返回值: TRUE  成功
         FALSE 失败
*其他  : 向最快服务发送数据
*****************************************************************************/
#ifndef WIN32
	JVSERVER_API int JVN_RegCard(char chGroup[4], unsigned char *pBuffer,int nSize);
#else
	JVSERVER_API bool __stdcall	JVN_RegCard(char chGroup[4], BYTE *pBuffer,int nSize);
#endif

/****************************************************************************
*名称  : JVN_SendAndRecvS
*功能  : 与最快服务器交互
*参数  : [IN] chGroup       分组号，形如"A" "AAAA"
         [IN] pBuffer       待发数据内容
         [IN] nSize         待发数据总长度
         [OUT] pRecvBuffer  接收数据缓冲，由调用者分配
         [IN/OUT] &nRecvLen 传入接收缓冲长度，返回实际数据长度
         [IN] nTimeOut      超时时间(秒)
*返回值: TRUE  成功
FALSE 失败
*其他  : 向最快服务发送数据
*****************************************************************************/
#ifndef WIN32
	JVSERVER_API int JVN_SendAndRecvS(char chGroup[4],unsigned char *pBuffer,int nSize, 
		                               unsigned char *pRecvBuffer,int *nRecvLen,int nTimeOut);
#else
	JVSERVER_API bool __stdcall	JVN_SendAndRecvS(char chGroup[4], BYTE *pBuffer,int nSize, 
		                                         BYTE *pRecvBuffer,int &nRecvLen,int nTimeOut);
#endif

/****************************************************************************
*名称  : JVN_StartWebServer
*功能  : 开启web服务
*参数  : [IN] chHomeDir   目的文件所在本地路径 如"D:\\test"
         [IN] chDefIndex  目的文件名(本地)
		 [IN] chLocalIP   本地ip
         [IN] nPort       web服务端口
*返回值: TRUE  成功
		 FALSE 失败
*其他  : web服务功能即返回目的文件给客户端
*****************************************************************************/
#ifndef WIN32
	JVSERVER_API int JVN_StartWebServer(char chHomeDir[256], char chDefIndex[256], char chLocalIP[30], int nPort);
#else
	JVSERVER_API bool __stdcall JVN_StartWebServer(char chHomeDir[256], char chDefIndex[256], char chLocalIP[30], int nPort);
#endif

/****************************************************************************
*名称  : JVN_StartSelfWebServer
*功能  : 开启自定义web服务(OEM)
*参数  : [IN] chHomeDir   目的文件所在本地路径 如"D:\\test"
         [IN] chDefIndex  目的文件名(本地)
		 [IN] chLocalIP   本地ip
         [IN] nPort       web服务端口
		 [IN] chSelfWebPos   自定义网站控件index文件位置 如"www.afdvr.com/cloudsee"
		 [IN] bOnlyUseLocal  仅使用本地web服务，不使用外网网站
*返回值: TRUE  成功
		 FALSE 失败
*其他  : web服务功能即返回目的文件给客户端
*****************************************************************************/
#ifndef WIN32
	JVSERVER_API int JVN_StartSelfWebServer(char chHomeDir[256], char chDefIndex[256], 
		                                     char chLocalIP[30], int nPort,
											 char chSelfWebPos[500], int bOnlyUseLocal);
#else
	JVSERVER_API bool __stdcall JVN_StartSelfWebServer(char chHomeDir[256], char chDefIndex[256], 
		                                               char chLocalIP[30], int nPort,
													   char chSelfWebPos[500], BOOL bOnlyUseLocal);
#endif

/****************************************************************************
*名称  : JVN_StopWebServer
*功能  : 停止web服务
*参数  : 无
*返回值: 无
*其他  : 无
*****************************************************************************/
#ifndef WIN32
	JVSERVER_API void JVN_StopWebServer();
#else
	JVSERVER_API void __stdcall JVN_StopWebServer();
#endif

/****************************************************************************
*名称  : JVN_Command
*功能  : 运行特定指令，要求SDK执行特定操作
*参数  : [IN] nChannel  本地通道 ==0时对所有音视频通道有效(不包括特殊通道)
         [IN] nCMD  指令类型
*返回值: 无
*其他  : 支持的指令参看类型定义
         目前仅支持：CMD_TYPE_CLEARBUFFER
		 主控端进行了某个操作，如果希望客户端能立即更新到当前最新的数据，可执行该指令；
*****************************************************************************/
#ifndef WIN32
	JVSERVER_API void JVN_Command(int nChannel, int nCMD);
#else
	JVSERVER_API void __stdcall JVN_Command(int nChannel, int nCMD);
#endif
	
/****************************************************************************
*名称  : JVN_StartLANSerchServer
*功能  : 开启局域网搜索服务
*参数  : [IN] nChannelCount 当前设备总的通道数
         [IN] nPort         服务端口号(<=0时为默认9103,建议使用默认值与分控端统一)
*返回值: 无
*其他  : 无
*****************************************************************************/
#ifndef WIN32
	JVSERVER_API int JVN_StartLANSerchServer(int nPort);
#else
	JVSERVER_API bool __stdcall JVN_StartLANSerchServer(int nPort);
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
*名称  : JVN_SetLocalNetCard
*功能  : 设置哪一张网卡 eth0,eth1,... for linux or 0, 1, 2,...for win  
*参数  : [IN] strCardName   网卡
*返回值: 成功 TRUE ,失败 FALSE
*日期  : 2012 5
*****************************************************************************/
#ifndef WIN32
	JVSERVER_API int JVN_SetLocalNetCard(char* strCardName);
#else
	JVSERVER_API BOOL __stdcall JVN_SetLocalNetCard(char* strCardName);
#endif

/****************************************************************************
*名称  : JVN_EnablePCTCPServer
*功能  : 开启或关闭PC用户的TCP服务
*参数  : [IN] bEnable 开启/关闭
*返回值: TRUE  成功
		 FALSE 失败
*其他  : TCP服务功能接收分控以TCP方式连接，以TCP方式向分控发送数据；
         目前中维分控都未使用该TCP服务,没有特殊需要可不使用该功能；
*****************************************************************************/
#ifndef WIN32
	JVSERVER_API int JVN_EnablePCTCPServer(int bEnable);
#else
	JVSERVER_API bool __stdcall JVN_EnablePCTCPServer(BOOL bEnable);
#endif

/****************************************************************************
*名称  : JVN_EnableMOServer
*功能  : 开启/关闭某通道的手机服务 
*参数  : [IN] nChannel   服务通道号 >=1 当=0时开启或关闭所有通道的手机服务
         [IN] bEnable    TRUE为开启；FALSE为关闭
		 [IN] nOpenType  服务开启类型：请置为0;bEnable为TRUE时有效；
		 [IN] bSendVSelf  是否单独发送手机视频数据，如果为TRUE，则手机数据和PC数据完全隔离，
		                 手机数据必须用JVN_SendMOData发送；否则手机视频数据将不需要单独发送；
						 如果把手机当分控用，给手机和给分控的是相同的码流，bSendVSelf=FALSE即可；
		 [IN] bProtocol  是否用自定义协议,当bSendVSelf时有效；
		                 TRUE时，JVN_SendMOData的数据打包需要单独添加头尾，使手机端能区别出该数据，
						         旧版主控发送的JPG数据和标准H264数据就是这种发送方式；
						 FALSE时，JVN_SendMOData的数据打包格式和分控数据一致，与分控数据的区别仅仅是数据内容，
						         如果把手机当分控用，但给手机的数据是单独的码流，可以使用该方式；
								 
*返回值: 无
*其他  : 该函数只对JVN_StartChannel开启了的通道起作用；JVN_StopChannel之后需要重新启用手机服务；
         没开启的通道将不能接受手机连接；
         重复调用将以最后一次调用为准；
		 由于旧版分控协议不同，是完全作为分控使用，该功能不能将其区分和禁用；
		 使用该版主控时公司产品已可以完全支持h264码流，数据和分控相同，不再支持JGP数据，
		 仅将手机服务开启即可；
		 <*****使用建议*****>：
		 1.如果给分控的数据和给手机的数据完全相同，则开启方式为JVN_EnableMOServer(0, TRUE, 0, FALSE, FALSE);
		 2.如果给分控的数据和给手机的数据格式相同，但是一个独立的码流，则开启方式为JVN_EnableMOServer(0, TRUE, 0, TRUE, FALSE);
		 3.如果给分控的数据和给手机的数据格式不同，需要让手机端单独处理，则开启方式为JVN_EnableMOServer(0, TRUE, 0, TRUE, TRUE);
		 理论上给手机的数据与分控数据分开，采用小帧率小码流，用第2种方式效果最佳；
*****************************************************************************/
#ifndef WIN32
	JVSERVER_API int JVN_EnableMOServer(int nChannel, int bEnable, int nOpenType, int bSendVSelf, int bProtocol);
#else
	JVSERVER_API BOOL __stdcall	JVN_EnableMOServer(int nChannel, BOOL bEnable, int nOpenType, BOOL bSendVSelf, BOOL bProtocol);
#endif

/****************************************************************************
*名称  : JVN_SendMOData
*功能  : 发送手机数据，对TYPE_MO_TCP/TYPE_MO_UDP连接有效 
*参数  : [IN] nChannel   服务通道号 >=1
         [IN] uchType    数据类型：视频:自定义类型;
         [IN] pBuffer    待发数据内容,视频/自定义数据时有效
		 [IN] nSize      待发数据长度,视频/自定义数据时有效
*返回值: 无
*其他  : 以通道为单位，向通道TYPE_MO_TCP/TYPE_MO_UDP连接的所有手机发送一些自定义数据；
         由于JGP数据的淘汰，手机数据和PC分控数据已完全相同，该接口不支持JPG数据；
*****************************************************************************/
#ifndef WIN32
	JVSERVER_API void JVN_SendMOData(int nChannel,unsigned char uchType,unsigned char *pBuffer,int nSize);
#else
	JVSERVER_API void __stdcall	JVN_SendMOData(int nChannel,unsigned char uchType,BYTE *pBuffer,int nSize);
#endif

/****************************************************************************
*名称  : JVN_StartStreamService
*功能  : 开启流媒体服务
*参数  : [IN] nChannel   服务通道号 >=1
         [IN] pSServerIP    流媒体服务器IP;
         [IN] nPort   流媒体服务器端口
	     [IN] bAutoConnect 开启服务失败是否自动重连
*返回值: 成功
         失败
*其他  : 当通道以普通方式运行时，流媒体服务器有效；
         当通道以中维网络直播方式运行时，流媒体服务器无效；
*****************************************************************************/
#ifndef WIN32
	JVSERVER_API int JVN_StartStreamService(int nChannel,char *pSServerIP,int nPort,int bAutoConnect);
#else
	JVSERVER_API BOOL __stdcall JVN_StartStreamService(int nChannel,char *pSServerIP,int nPort,BOOL bAutoConnect);
#endif

/****************************************************************************
*名称  : JVN_StopStreamService
*功能  : 停止流媒体服务
*参数  : [IN] nChannel   服务通道号 >=1
*返回值: 无
*****************************************************************************/
#ifndef WIN32
	JVSERVER_API void JVN_StopStreamService(int nChannel);
#else
	JVSERVER_API void __stdcall JVN_StopStreamService(int nChannel);
#endif

/****************************************************************************
*名称  : JVN_GetStreamServiceStatus
*功能  : 查询流媒体服务状态
*参数  : [IN] nChannel   服务通道号 >=1
         [OUT] pSServerIP    流媒体服务器IP;
         [OUT] nPort   流媒体服务器端口
		 [OUT] bAutoConnect 流媒体服务是否正在自动重连
*返回值: 流媒体服务是否成功开启
*日期  : 2012 2
*****************************************************************************/
#ifndef WIN32
	JVSERVER_API int JVN_GetStreamServiceStatus(int nChannel,char *pSServerIP,int *nPort,int *bAutoConnect);
#else
	JVSERVER_API BOOL __stdcall JVN_GetStreamServiceStatus(int nChannel,char *pSServerIP,int *nPort,BOOL *bAutoConnect);
#endif

/****************************************************************************
*名称  : JVN_SetDomainName 
*功能  : 设置新的域名，系统将从其获取服务器列表
*参数  : [IN]  pchDomainName     域名
         [IN]  pchPathName       域名下的文件路径名 形如："/down/YSTOEM/yst0.txt"
*返回值: TRUE  成功
         FALSE 失败
*其他  : 系统初始化(JVN_InitSDK)完后设置
*****************************************************************************/
#ifndef WIN32
	JVSERVER_API int JVN_SetDomainName(char *pchDomainName,char *pchPathName);
#else
	JVSERVER_API BOOL __stdcall	JVN_SetDomainName(char *pchDomainName,char *pchPathName);
#endif

/****************************************************************************
*名称  : JVN_SetDeviceName
*功能  : 设置本地设备别名 
*参数  : [IN] chDeviceName   设备别名
*返回值: 无
*其他  : 为设备起一个别名，局域网设备搜索中可作搜索和显示使用；
         重复调用以最后一次调用为有效；
*****************************************************************************/
#ifndef WIN32
	JVSERVER_API void JVN_SetDeviceName(char chDeviceName[100]);
#else
	JVSERVER_API void __stdcall	JVN_SetDeviceName(char chDeviceName[100]);
#endif

/****************************************************************************
*名称  : JVN_SetLocalFilePath
*功能  : 自定义本地文件存储路径，包括日志，生成的其他关键文件等 
*参数  : [IN] chLocalPath  路径 形如："C:\\jovision"  其中jovision是文件夹
*返回值: 无
*其他  : 参数使用内存拷贝时请注意初始化，字符串需以'\0'结束
*****************************************************************************/
#ifndef WIN32
	JVSERVER_API int JVN_SetLocalFilePath(char chLocalPath[256]);
#else
	JVSERVER_API BOOL __stdcall	JVN_SetLocalFilePath(char chLocalPath[256]);
#endif

/****************************************************************************
*名称  : JVN_StartBroadcastServer
*功能  : 开启局域网广播服务
*参数  : [IN] nPort    服务端口号(<=0时为默认9106,建议使用默认值与分控端统一)
         [IN] BCData   广播数据回调函数
*返回值: 成功/失败
*其他  : 无
*****************************************************************************/
#ifndef WIN32
	JVSERVER_API int JVN_StartBroadcastServer(int nPort, FUNC_SBCDATA_CALLBACK BCData);
#else
	JVSERVER_API BOOL __stdcall JVN_StartBroadcastServer(int nPort, FUNC_SBCDATA_CALLBACK BCData);
#endif

/****************************************************************************
*名称  : JVN_StopBroadcastServer
*功能  : 停止局域网广播服务
*参数  : 无
*返回值: 无
*其他  : 无
*****************************************************************************/
#ifndef WIN32
	JVSERVER_API void JVN_StopBroadcastServer();
#else
	JVSERVER_API void __stdcall JVN_StopBroadcastServer();
#endif

/****************************************************************************
*名称  : JVN_BroadcastRSP
*功能  : 局域网广播响应
*参数  : [IN] nBCID  广播标识，取自回调函数
         [IN] pBuffer 广播净载数据
		 [IN] nSize   广播净载数据长度
		 [IN] nDesPort 广播目标端口，取自回调函数，或是与分控约定固定
*返回值: 成功/失败
*其他  : 无
*****************************************************************************/
#ifndef WIN32
	JVSERVER_API int JVN_BroadcastRSP(int nBCID, unsigned char *pBuffer, int nSize, int nDesPort);
#else
	JVSERVER_API BOOL __stdcall JVN_BroadcastRSP(int nBCID, BYTE *pBuffer, int nSize, int nDesPort);
#endif

/****************************************************************************
*名称  : JVN_SendPlay
*功能  : 发送回放MP4数据 
*参数  : [IN] nClientID   连接号
         [IN] uchType     类型
		 [IN] nConnectionType     连接类型
		 [IN] ucFrameType    帧类型	JVN_DATA_I JVN_DATA_S...
         [IN] pBuffer    待发数据内容,保留
		 [IN] nSize      待发数据长度,保留
         [IN] nWidth    宽度
		 [IN] nHeight      高度
		 [IN] nTotalFram      总帧数
*返回值: 无
*其他  : 向通道连接的某个具体分控发送数据
*****************************************************************************/
#ifndef WIN32
	JVSERVER_API void JVN_SendPlay(int nClientID,unsigned char uchType,int nConnectionType,unsigned char ucFrameType,unsigned char *pBuffer,int nSize,int nWidth,int nHeight,int nTotalFram);
#else
	JVSERVER_API void __stdcall	JVN_SendPlay(int nClientID,BYTE uchType,int nConnectionType,BYTE ucFrameType,BYTE *pBuffer,int nSize,int nWidth,int nHeight,int nTotalFram);
#endif

/****************************************************************************
*名称  : JVN_EnableLANToolServer
*功能  : 开启或关闭局域网生产工具服务
*参数  : [IN] bEnable         开启/关闭
         [IN] nPort           本地使用的端口，=0时默认为9104
         [IN] LanToolCallBack 工具回调函数
*返回值: TRUE  成功
		 FALSE 失败
*其他  : 局域网生产工具会向本地询问号码信息，并且反馈生产相关的附加信息
         信息详细说明请参考回调函数说明。
*****************************************************************************/
#ifndef WIN32
	JVSERVER_API int JVN_EnableLANToolServer(int bEnable, int nPort, FUNC_LANTOOL_CALLBACK LanToolCallBack);
#else
	JVSERVER_API bool __stdcall JVN_EnableLANToolServer(BOOL bEnable, int nPort, FUNC_LANTOOL_CALLBACK LanToolCallBack);
#endif

/****************************************************************************
*名称  : JVN_RegDownLoadFileName
*功能  : 注册回调函数，用于调用者特殊处理远程下载文件名
*参数  : [IN] DLFNameCallBack 远程下载文件名处理回调函数
*返回值: 无
*其他  : 普通产品不必使用，有特殊要求的可对客户端的下载文件名做二次处理
*****************************************************************************/
#ifndef WIN32
	JVSERVER_API int JVN_RegDownLoadFileName(FUNC_DLFNAME_CALLBACK DLFNameCallBack);
#else
	JVSERVER_API bool __stdcall JVN_RegDownLoadFileName(FUNC_DLFNAME_CALLBACK DLFNameCallBack);
#endif

/****************************************************************************
*名称  : JVN_SetIPCWANMode
*功能  : 启用IPC外网特殊处理模式
*参数  : 无
*返回值: 无
*其他  : 普通产品不必使用，有特殊要求的大码流产品可以使用；
         启用后，外网传输将采用特别处理来减少对内网连接的干扰；
		 在JVN_InitSDK后，JVN_StartChannel前调用即可；
*****************************************************************************/
#ifndef WIN32
	JVSERVER_API void JVN_SetIPCWANMode();
#else
	JVSERVER_API void __stdcall JVN_SetIPCWANMode();
#endif

/****************************************************************************
*名称  : JVN_SetDeviceInfo
*功能  : 设置设备信息
*参数  : [IN] pstInfo 设备信息结构体
      	 [IN] nSize   设备信息结构体大小,用来前后兼容
	     [IN] nflag   设置选项标志 
*返回值: 成功返回0，失败返回-1
*其他  : 如设置设备支持的网络模式为有线+wifi，当前正在使用的是wifi，则参数是: 
		 nflag = DEV_SET_NET;//设置设备支持的网络模式
		 pstInfo->nNetMod= NET_MOD_WIFI | NET_MOD_WIRED;
         pstInfo->nNetMod= NET_MOD_WIFI;//设备当前使用的是wifi
*****************************************************************************/
#ifndef WIN32
	JVSERVER_API int JVN_SetDeviceInfo(STDEVINFO *pstInfo, int nSize,int nflag);
#else
	JVSERVER_API int JVN_SetDeviceInfo(STDEVINFO *pstInfo, int nSize,int nflag);
#endif
	
/****************************************************************************
*名称  : JVNS_GetDeviceInfo
*功能  : 获取设备信息
*参数  : [OUT] pstInfo 设备信息结构体
	     [IN]  nSize   设备信息结构体大小,用来前后兼容
*返回值: 成功返回0，失败返回-1
*****************************************************************************/
#ifndef WIN32
	JVSERVER_API int JVN_GetDeviceInfo(STDEVINFO *pstInfo,int nSize);
#else
	JVSERVER_API int JVN_GetDeviceInfo(STDEVINFO *pstInfo,int nSize);
#endif

/****************************************************************************
*名称  : JVN_GetNetSpeedNow
*功能  : 获取当前网络状况
*参数  : [IN]	nChannel	通道号
		 [IN]	nClientID	连接号		 
		 [IN]	nInterval	计算间隔, 该参数小于16时无效，默认最小为16秒
*返回值: 成功返回值>0，失败返回-1
*其他  : 返回当前网络状况值单位Bps，失败时原因主要有视频传输没有开启，或者参数错误
*****************************************************************************/
#ifndef WIN32
	JVSERVER_API int JVN_GetNetSpeedNow(int nChannel, int nClientID, int nInterval);
#else
	JVSERVER_API int __stdcall JVN_GetNetSpeedNow(int nChannel, int nClientID, int nInterval = 16);
#endif

/****************************************************************************
*名称  : JVN_SetLSPrivateInfo
*功能  : 设置本地自定义信息，用于设备搜索
*参数  : [IN]	chPrivateInfo  自定义信息
		 [IN]	nSize          自定义信息长度		 
*返回值: 无
*其他  : 可重复调用，以最后一次调用为准，之前的内容会被覆盖；
*****************************************************************************/
#ifndef WIN32
	JVSERVER_API void JVN_SetLSPrivateInfo(char chPrivateInfo[500], int nSize);
#else
	JVSERVER_API void __stdcall JVN_SetLSPrivateInfo(char chPrivateInfo[500], int nSize);
#endif

#endif
