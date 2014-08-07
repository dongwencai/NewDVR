#ifndef __COMMON_H__
#define __COMMON_H__

#ifndef __IN
#define __IN
#define __OUT
#endif

#define inArea(pos,rect)	((pos.s32X>rect.lTop.s32X&&pos.s32X<rect.rBottom.s32X)&&(pos.s32Y>rect.lTop.s32Y&&pos.s32Y<rect.rBottom.s32Y))
#define GUI_COUNTOF(a) (sizeof(a) / sizeof(a[0]))
#define HIWORD(a)       ((a&0xffff0000)>>16)
#define LOWWORD(a)      (a&0xffff)
#define HIBYTE(a)       ((a&0xff00)>>8)
#define LOWBYTE(a)      (a&0xff)
extern void GUI__IntersectRect(PRECT pDest, const PRECT pr0);

#endif
