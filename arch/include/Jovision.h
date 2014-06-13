#if !defined(_JOVISION_H_)
#define _JOVISION_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

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


//UDP通信

///////////////////////////////////////////////////
//服务端接口
///////////////////////////////////////////////////

//服务端回调函数，将客户端发送到的数据回调给应用程序
//参数
//nServerSeq 服务号码 唯一的数
//pClientIP 数据来源的IP
//nPort 数据来源的端口
//pBuff 数据
//nLen 数据长度
//
typedef bool (*CALL_SERVER_RECEIVE)(int nServerSeq,char* pClientIP,int nPort,char* pBuff,int nLen);


//初始化函数，创建一服务通道
//参数
//nServerSeq 服务号码，唯一
//nPort 服务端口
//CALL_SERVER_RECEIVE 数据接收的回调函数
//
//返回
//true可以使用，false错误，错误信息查看日志
#ifndef WIN32
JVSERVER_API bool Udp_Server_Init(int nServerSeq,int nPort,CALL_SERVER_RECEIVE call_SERVER_RECEIVE);
#else
JVSERVER_API bool __stdcall	Udp_Server_Init(int nServerSeq,int nPort,CALL_SERVER_RECEIVE call_SERVER_RECEIVE);
#endif

//服务端发送函数
//参数
//nServerSeq 服务号码，初始化时设定的号码
//pClientIP 客户端IP	在回调函数中获取到的数值
//nPort 客户端端口	在回调函数中获取到的数值
//pBuff 需要发送的数据
//nLen 数据长度
//
#ifndef WIN32
JVSERVER_API void Udp_Server_Send(int nServerSeq,char* pClientIP,int nPort,char* pBuff,int nLen);
#else
JVSERVER_API void __stdcall	Udp_Server_Send(int nServerSeq,char* pClientIP,int nPort,char* pBuff,int nLen);
#endif


//服务端停止1服务
//参数
//nServerSeq 需要停止的服务编号
//
//返回
//true 结束，false 失败
#ifndef WIN32
JVSERVER_API bool Udp_Server_Release(int nServerSeq);
#else
JVSERVER_API bool __stdcall	Udp_Server_Release(int nServerSeq);
#endif



///////////////////////////////////////////////////
//客户端接口
///////////////////////////////////////////////////

//客户端回调函数，将客户端发送到的数据回调给应用程序

//nClientSeq 客户端号码 唯一的数
//pServerIP 数据来源的IP
//nPort数据来源的端口
//pBuff数据
//nLen数据长度
//
typedef bool (*CALL_CLIENT_RECEIVE)(int nClientSeq,char* pServerIP,int nPort,char* pBuff,int nLen);


//初始化函数，创建一客户端通道
//参数
//nClientSeq 客户端号码，唯一
//pServerIP 服务器IP
//nPort 服务端口
//CALL_CLIENT_RECEIVE 数据接收的回调函数
//
//返回
//true可以使用，false错误，错误信息查看日志
#ifndef WIN32
JVSERVER_API bool Udp_Client_Init(int nClientSeq,char* pServerIP,int nPort,CALL_CLIENT_RECEIVE call_CLIENT_RECEIVE);
#else
JVSERVER_API bool __stdcall	Udp_Client_Init(int nClientSeq,char* pServerIP,int nPort,CALL_CLIENT_RECEIVE call_CLIENT_RECEIVE);
#endif

//客户端发送函数
//参数
//nClientSeq 客户端号码，初始化时设定的号码
//pBuff 需要发送的数据
//nLen 数据长度
//
#ifndef WIN32
JVSERVER_API void Udp_Client_Send(int nClientSeq,char* pBuff,int nLen);
#else
JVSERVER_API void __stdcall	Udp_Client_Send(int nClientSeq,char* pBuff,int nLen);
#endif

//停止通道通信
//参数
//nClientSeq 需要停止的编号
//
//返回
//true 结束，false 失败
#ifndef WIN32
JVSERVER_API bool Udp_Client_Release(int nClientSeq);
#else
JVSERVER_API bool __stdcall	Udp_Client_Release(int nClientSeq);
#endif

////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////

//TCP通信
//回调函数参考UDP通信

///////////////////////////////////////////////////
//服务端接口
///////////////////////////////////////////////////

//初始化函数，创建一服务通道
//参数
//nServerSeq 服务号码，唯一
//nPort 服务端口
//CALL_SERVER_RECEIVE 数据接收的回调函数
//
//返回
//true可以使用，false错误，错误信息查看日志
#ifndef WIN32
JVSERVER_API bool Tcp_Server_Init(int nServerSeq,int nPort,CALL_SERVER_RECEIVE call_SERVER_RECEIVE);
#else
JVSERVER_API bool __stdcall	Tcp_Server_Init(int nServerSeq,int nPort,CALL_SERVER_RECEIVE call_SERVER_RECEIVE);
#endif

//服务端发送函数
//参数
//nServerSeq 服务号码，初始化时设定的号码
//pClientIP 客户端IP	在回调函数中获取到的数值
//nPort 客户端端口	在回调函数中获取到的数值
//pBuff 需要发送的数据
//nLen 数据长度
//
#ifndef WIN32
JVSERVER_API void Tcp_Server_Send(int nServerSeq,char* pClientIP,int nPort,char* pBuff,int nLen);
#else
JVSERVER_API void __stdcall	Tcp_Server_Send(int nServerSeq,char* pClientIP,int nPort,char* pBuff,int nLen);
#endif


//服务端停止1服务
//参数
//nServerSeq 需要停止的服务编号
//
//返回
//true 结束，false 失败
#ifndef WIN32
JVSERVER_API bool Tcp_Server_Release(int nServerSeq);
#else
JVSERVER_API bool __stdcall	Tcp_Server_Release(int nServerSeq);
#endif


///////////////////////////////////////////////////
//客户端接口
///////////////////////////////////////////////////

//客户端回调函数，将客户端发送到的数据回调给应用程序


//初始化函数，创建一客户端通道
//参数
//nClientSeq 客户端号码，唯一
//pServerIP 服务器IP
//nPort 服务端口
//CALL_CLIENT_RECEIVE 数据接收的回调函数
//
//返回
//true可以使用，false错误，错误信息查看日志
#ifndef WIN32
JVSERVER_API bool Tcp_Client_Init(int nClientSeq,char* pServerIP,int nPort,CALL_CLIENT_RECEIVE call_CLIENT_RECEIVE);
#else
JVSERVER_API bool __stdcall	Tcp_Client_Init(int nClientSeq,char* pServerIP,int nPort,CALL_CLIENT_RECEIVE call_CLIENT_RECEIVE);
#endif

//客户端发送函数
//参数
//nClientSeq 客户端号码，初始化时设定的号码
//pBuff 需要发送的数据
//nLen 数据长度
//
#ifndef WIN32
JVSERVER_API void Tcp_Client_Send(int nClientSeq,char* pBuff,int nLen);
#else
JVSERVER_API void __stdcall	Tcp_Client_Send(int nClientSeq,char* pBuff,int nLen);
#endif

//停止1通道通信
//参数
//nClientSeq 需要停止的编号
//
//返回
//true 结束，false 失败
#ifndef WIN32
JVSERVER_API bool Tcp_Client_Release(int nClientSeq);
#else
JVSERVER_API bool __stdcall	Tcp_Client_Release(int nClientSeq);
#endif

//nServerSeq nClientSeq 可以相同，但所有的nServerSeq，不可以重复，所有的nClientSeq不可以重复

#endif


