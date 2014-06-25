#ifndef __SW_FB_H__
#define __SW_FB_H__

#include "sw_types.h"

typedef struct _FRAMEBUFFER
{
	S32			hFB;				//句柄
	U8 *		pMappedAddr;		//映射到应用层的用户态地址
	U8 *		pPhyAddr;			//物理地址
	U32		    u32MemSize;			//内存大小
}FRAMEBUFFER, *PFRAMEBUFFER;
typedef struct
{
	S32 nW;
	S32 nH;
}SIZE_T;
typedef struct
{
    FRAMEBUFFER *pWinFb;            //窗口层fb
    FRAMEBUFFER *pHdOsdFb;          //HD的osd层
    FRAMEBUFFER *pSdOsdFb;          //SD的osd层
    FRAMEBUFFER *pResFb;            //缓存图片资源的fb
    U32         nCurDev;			//显示gui的设备
    
    SIZE_T      pSzWin;				//窗口层的大小
    U32 tcClear;				 
    
    
}DC_S;

#endif
