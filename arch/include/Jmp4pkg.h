#ifndef _JVS_PKG_H
#define _JVS_PKG_H

/************************************************************
Copyright (C), 2012-2014, Jovision Tech. co., Ltd.
File Name:		Jmp4pkg.h
Author:			王修扬
Version:		Ver 1.0.1.10
Description:	
				封装功能:
				1. 实现标准h264码流/amr码流/g711码流封装
				成mp4格式文件, 一次只封装一帧

				2.加入封装mp4时,将帧信息保存为临时文件，
				用于解封正在封装mp4功能(边录边放)

				解封装功能:
				1. 实现标准h264码流和amr\g.711码流的
				mp4文件解封装, 一次只解封一个码流

				2. 从临时文件中读取帧在mp4文件中的信息，
				用于解封正在封装的mp4文件 
				

************************************************************/

#ifdef WIN32

	typedef __int64					int64_t;
	typedef unsigned __int64		uint64_t;


	#ifdef __cplusplus
	#define JVS_API extern "C" __declspec(dllexport)
	#else
	#define JVS_API __declspec(dllexport)
	#endif

#else

	typedef long long				int64_t;
	typedef unsigned long long		uint64_t;	


#ifndef TRUE
	typedef int BOOL;
	#define TRUE  1
	#define FALSE 0
#endif

	#ifdef __cplusplus
	#define JVS_API extern "C"
	#else
	#define JVS_API
	#endif

#endif

// 要封装的音频流类型
#define JVS_ACODEC_SAMR		0
#define JVS_ACODEC_ALAW		1
#define JVS_ACODEC_ULAW		2

// 在JP_PackageOneFrame 会比较avPkt.type
#define JVS_PKG_VIDEO		1			// 封装视频类型
#define JVS_PKG_AUDIO		2			// 封装音频类型

// mp4 sps\pps, 由h264打开编码器后传入,
// 1. 传入时sps和pps要去掉前面的 00 00 01/ 00 00 00 01
// 2. 也可以不传入 置 iSpsSize = 0; pPps = NULL(效率降低?) 
typedef struct _MP4_AVCC
{
	int					iSpsSize;			// h264 sps大小
	int					iPpsSize;			// h264 pps大小
	unsigned char *		pSps;				// h264 sps
	unsigned char *		pPps;				// h264 pps
	
}MP4_AVCC;

// 封装开始时输入视频参数
typedef struct _PKG_VIDEO_PARAM				
{											
	
	int					iFrameWidth;		// 帧宽度
	int					iFrameHeight;		// 帧高度
	float				fFrameRate;			// 帧速
	// 如果不传入sps\pps, 必须将pSps = NULL, iSpsSize = 0;
	MP4_AVCC			avcC;			 
																			
}PKG_VIDEO_PARAM, * PPKG_VIDEO_PARAM;


// 封装过程中输入(视频或音频)
typedef struct _AV_PACKET
{
	unsigned int		iType;				// 封装帧类型(视频或音频)
	unsigned char *		pData;				// 封装数据指针
	unsigned int		iSize;				// 封装数据大小
	int64_t				iPts;				// 没有b帧传0 由编码器输出
	int64_t				iDts;				// 没有b帧传0 由编码器输出
}AV_PACKET, * PAV_PACKET;


typedef struct _MP4_PKG *	MP4_PKG_HANDLE;	//封装句柄, 由Open返回

/**********************************************************************************
Function:		JP_OpenPackage
Description:	打开封装器和JP_ClosePackage成对使用
Param:			所有参数都由调用者都输入 	
				PPKG_VIDEO_PARAM pVideoParam	// 封装视频输入参数 wdith, sps..
				BOOL			 bWriteVideo	// 是否写入视频
				BOOL			 bWriteAudio	// 是否写入音频
				char *			 pszmp4file     // 保存为mp4文件名
				char *			 pszIdxFile		// 辅助解封的文件,保存帧在mp4文件中的索引信息,
												// 解封正在封装过程的mp4文件
												// (扩展名.jdx, 文件名和mp4相同)
				int				 iAcodec		// 要封装的音频流类型
				int				 iChannel		// linux中需要传入的通道号

												
Return:			返回封装器句柄, NULL,表示失败,否则表示成功      	  
**********************************************************************************/ 
JVS_API	MP4_PKG_HANDLE	JP_OpenPackage		(PPKG_VIDEO_PARAM	pVideoParam, 
											 BOOL				bWriteVideo, 
											 BOOL				bWriteAudio, 
											 char *				pszmp4file,
											 char *				pszIdxFile,
											 int				iAcodec,
											 int				iChannel);



/**********************************************************************************
Function:		JP_ClosePackage
Description:	关闭封装器和JP_OpenPackage成对使用
Param:			MP4_PKG_HANDLE	h		// 封装器句柄
Return:			无	  
**********************************************************************************/ 
JVS_API	void			JP_ClosePackage		(MP4_PKG_HANDLE h);



/**********************************************************************************
Function:		JP_PackageOneFrame
Description:	封装一帧h264或amr数据
Param:			MP4_PKG_HANDLE	h		// 封装器句柄
				PAV_PACKET pAVPkt		// h264或amr帧数据结构体

Return:			返回 TURE, 表示成功，返回FALSE,表示失败	  
**********************************************************************************/ 
JVS_API	BOOL			JP_PackageOneFrame	(MP4_PKG_HANDLE h, PAV_PACKET pAVPkt);



#ifndef	WIN32
/**********************************************************************************
Function:		JP_InitSDK
Description:	每路分配iSize大小的缓存, linux中先调用且只调用一次
Param:			int			iSize		// 每路都分配的大小
				int			iCount		// 分配的路数

Return:			返回 TURE, 表示成功，返回FALSE,表示失败	  
**********************************************************************************/ 
JVS_API	BOOL			JP_InitSDK(int iSize, int iCount);


/**********************************************************************************
Function:		JP_ReleaseSDK
Description:	释放JP_InitSDK分配的缓存
Param:			int			iSize		// 每路都分配的大小
				int			iCount		// 分配的路数

Return:			返回 TURE, 表示成功，返回FALSE,表示失败	  
**********************************************************************************/ 
JVS_API	void			JP_ReleaseSDK();
#endif






// 在JP_UnpkgOneFrame 会比较avUnpkt.type
#define  JVS_UPKT_VIDEO		1			// 解封视频类型
#define  JVS_UPKT_AUDIO		2			// 解音视频类型

// 打开解封器返回mp4文件信息,由调用者维持此结构体
typedef struct _MP4_INFO
{
	char				szVideoMediaDataName[8];	// 视频编码名字 "avc1"
	unsigned int		iFrameWidth;				// 视频宽度
	unsigned int		iFrameHeight;				// 视频高度
	unsigned int		iNumVideoSamples;			// VideoSample个数
	double				dFrameRate;					// 帧速
	
	char				szAudioMediaDataName[8];	// 音频编码名字 "samr" "alaw" "ulaw"
	unsigned int		iNumAudioSamples;			// AudioSample个数

	
}MP4_INFO, *PMP4_INFO;

// 解封过程中输入(视频或音频),由调用者维持此结构体
typedef struct _AV_UNPKT
{
	unsigned int		iType;			// 解封类型	(输入)
	unsigned int		iSampleId;		// 解封Sample ID (输入) (1 -- iNumVideoSamples) 
	unsigned char *		pData;			// 解封输出数据指针 解封器只维持一次调用的数据
	unsigned int		iSize;			// 解封输出数据大小
	uint64_t			iSampleTime;	// 解封输出时间戳 JP_UnpkgOneFrame不返回这个值
	BOOL				bKeyFrame;		// 是否为关键帧(输出)
}AV_UNPKT, *PAV_UNPKT;

typedef struct _MP4_UPK *		MP4_UPK_HANDLE;	// 解封句柄 由open返回


/**********************************************************************************
Function:		JP_OpenUnpkg
Description:	打开解封器和JP_CloseUnpkg成对使用
Param:			
				char *			pszmp4file		// 要解封的文件名
				PMP4_INFO		pMp4Info		// 输出的AudioSample、AudioSample个数
				unsigned int 	iBufSize		// setvbuf参数
												// linux dvr中使用(若为0, 64kB)

Return:			返回解封器句柄, NULL, 表示失败,否则表示成功  	  
**********************************************************************************/ 
JVS_API MP4_UPK_HANDLE  JP_OpenUnpkg	(char *pszmp4file, PMP4_INFO pMp4Info, unsigned int iBufSize);



/**********************************************************************************
Function:		JP_CloseUnpkg
Description:	关闭解封器和JP_OpenUnpkg成对使用
Param:			
				MP4_PKG_HANDLE	h				// 解封器句柄
Return:			无  
**********************************************************************************/
JVS_API void			JP_CloseUnpkg	(MP4_UPK_HANDLE h);



/**********************************************************************************
Function:		JP_UnpkgOneFrame
Description:	解封一帧h264/amr/g711数据
Param:			MP4_UPK_HANDLE	h				// 解封器句柄
				PAV_UNPKT pAvUnpkt				// 解封h264或amr/g711帧数据结构体
				// if (pAvUnpkt->iSampleId <= 0 || pAvUnpkt->iSampleId > iNumVideoSamples)
				//	  return FALSE;

Return:			返回 TURE,表示成功，返回FALSE,表示失败	  
**********************************************************************************/ 
JVS_API BOOL			JP_UnpkgOneFrame(MP4_UPK_HANDLE h, PAV_UNPKT pAvUnpkt);



/**********************************************************************************
Function:		JP_UnpkgKeyFrame
Description:	查找给出帧号附近的关键帧
Param:			MP4_UPK_HANDLE	h				// 解封器句柄
				unsigned int	iVFrameNo		// 传入的视频帧号
				BOOL			bForward		// 表示查找方向, 
												// TRUE向前(往右), FALSE, 向后(往左) 

Return:			返回关键帧帧号	-2,表示出错,   -1, 表示没有找到  
**********************************************************************************/ 
JVS_API int				JP_UnpkgKeyFrame(MP4_UPK_HANDLE h, unsigned int iVFrameNo, BOOL bForward);







typedef struct _MP4_FILE *	MP4_FILE_HANDLE;		// 解封正在封装的mp4文件句柄

// 由调用者维持此结构体
typedef struct _MP4_CHECK
{
	BOOL			bNormal;						// TRUE, 表示已完成封装,FALSE, 封装正在进行中
	unsigned int	iDataStart;						// 解封mp4文件数据开始位置
}MP4_CHECK, *PMP4_CHECK;

// 打开mp4文件返回视频信息,由调用者维持此结构体
typedef struct _JP_MP4_INFO
{
	BOOL				bHasVideo;					// 是否有视频
	BOOL				bHasAudio;					// 是否有音频

	char				szVideoMediaDataName[8];	// 视频编码名字 "avc1"
	unsigned int		iFrameWidth;				// 视频宽度
	unsigned int		iFrameHeight;				// 视频高度
	double				dFrameRate;					// 帧速
	unsigned int		iNumVideoSamples;			// VideoSample个数, 打开mp4文件时返回当前总帧数

	char				szAudioMediaDataName[8];	// 音频编码名字 "samr" "alaw" "ulaw"
	unsigned int		iNumAudioSamples;			// AudioSample个数, 打开mp4文件时返回当前总帧数
}JP_MP4_INFO, *PJP_MP4_INFO;

/**********************************************************************************
Function:		JP_CheckFile
Description:	检查mp4是否为正在封装的文件，或是否为正常的文件
				要解封mp4文件,必须先调用这个接口，以决定调用哪一套API,
				如果 pMp4Check.bNormal = TRUE, 要调用上面的API,
				否则调用下面的API
Param:			
				char *			pszmp4file			// 要检查mp4文件名
				PMP4_CHECK		pMp4Check			// mp4文件的检查结果

Return:			TRUE,检查成功, FALSE, 检查失败  	  
**********************************************************************************/ 
JVS_API BOOL			JP_CheckFile	(char *pszmp4file, PMP4_CHECK pMp4Check);




/**********************************************************************************
Function:		JP_OpenFile
Description:	打开mp4文件和JP_CloseFile成对使用
Param:			
				char *			pszmp4file	// 要解封的文件名
				unsigned int	iDataStart	// 解封mp4文件数据开始位置
				char *			pszIdxFile	// 用于辅助解封的文件,此文件由封装库生成
				unsigned int 	iBufSize	// setvbuf参数
											// linux dvr中使用(若为0, 64kB)

Return:			返回解封器句柄, NULL,表示失败,否则表示成功  	  
**********************************************************************************/
JVS_API MP4_FILE_HANDLE	JP_OpenFile		(char *			pszmp4file, 
										 unsigned int	iDataStart,
										 char *			pszIdxFile,
										 PJP_MP4_INFO	pMp4Info,
										 unsigned int	iBufSize);



/**********************************************************************************
Function:		JP_ReadFile
Description:	解封一帧h264或amr数据
Param:			MP4_FILE_HANDLE	h			// 解封器句柄
				PAV_UNPKT		pAvUnpkt	// 解封h264或amr帧数据结构体

				// if (pAvUnpkt->iSampleId <= 0 || pAvUnpkt->iSampleId > iNumVideoSamples)
				//	  return 0;

Return:			0, 表示无数据可读,否则还有数据	  
**********************************************************************************/ 
JVS_API int				JP_ReadFile		(MP4_FILE_HANDLE mp4Handle, PAV_UNPKT pAvUnpkt);




/**********************************************************************************
Function:		JP_CloseFile
Description:	关闭解封器和JP_OpenFile成对使用
Param:			
				MP4_FILE_HANDLE	h			// 解封器句柄

Return:			无  
**********************************************************************************/
JVS_API void			JP_CloseFile	(MP4_FILE_HANDLE mp4Handle);




/**********************************************************************************
Function:		JP_ReadKeyFrame
Description:	查找给出帧号附近的关键帧
Param:			MP4_FILE_HANDLE	h					// 解封器句柄
				unsigned int	iVFrameNo			// 传入的视频帧号
				BOOL			bForward			// 表示查找方向, 
													// TRUE向前(往右), FALSE, 向后(往左) 

Return:			返回关键帧帧号	-2,表示出错,   -1, 表示没有找到  
**********************************************************************************/ 
JVS_API int				JP_ReadKeyFrame (MP4_FILE_HANDLE h, unsigned int iVFrameNo, BOOL bForward);



#endif	// _JVS_PKG_H
