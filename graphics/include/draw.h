#ifndef __DRAW_H__
#define __DRAW_H__
#include "hi_common.h"
#include "sw_types.h"

#define MK_LINE(x1,y1,x2,y2,fg,lineWidth)	\
{							\
	.pos_s={{x1,y1},{x2,y2}},	\
	.emCtrType=LINE,		\
	.ctrRes={0},			\
	.u32Value=lineWidth,	\
	.ctrColorInfo={fg,0},	\
}
#define MK_BOTTON(x1,y1,x2,y2,fg,bg)	\
{							\
	.pos_s={{x1,y1},{x2,y2}},	\
	.emCtrType=BUTTON,		\
	.ctrRes={0},			\
	.ctrColorInfo={fg,bg},	\
}
extern int drawLine(POINT_S begin_s,POINT_S end_s,U16 u16Color,int nLineWidth);
extern int drawRectangle(POINT_S leftTop_s,POINT_S rightBottom_s,U16 u16Color,int nLineWidth);

#endif