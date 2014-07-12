#ifndef __DRAW_H__
#define __DRAW_H__
#include "hi_common.h"
#include "sw_types.h"
#include "window.h"
#define MK_LINE(x1,y1,x2,y2,bg,lineWidth,action)	\
{							\
	.pos_s={{x1,y1},{x2,y2}},	\
	.emCtrType=LINE,		\
	.u32Value=lineWidth,	\
	.ctrlSkin_s={0,S_COLOR,bg},	\
	.cb=action,					\
}
#define MK_BOTTON(x1,y1,x2,y2,bg,action)	\
{							\
	.pos_s={{x1,y1},{x2,y2}},	\
	.emCtrType=BUTTON,		\
	.ctrlSkin_s={{0,S_COLOR,bg},{0,S_COLOR,0xfc00}},	\
	.cb=action,					\
}
#define MK_IMAGE(x1,y1,x2,y2,bg,action)		\
{											\
	.pos_s={{x1,y1},{x2,y2}},	\
	.emCtrType=IMAGE,		\
	.ctrlSkin_s={{0,S_COLOR,bg},{0,S_COLOR,0xfc00}},	\
	.cb=action,								\		
}
#define MK_LABLE(x1,y1,x2,y2,bg,action)		\
{											\
	.pos_s={{x1,y1},{x2,y2}},	\
	.emCtrType=LABEL,		\
	.ctrlSkin_s={{0,S_COLOR,bg},{0,S_COLOR,0xfc00}},	\
	.cb=action,								\		
}
extern int drawLine(POINT_S begin_s,POINT_S end_s,U16 u16Color,int nLineWidth);
extern int drawRectangle(POINT_S leftTop_s,POINT_S rightBottom_s,U16 u16Color,int nLineWidth);

#endif