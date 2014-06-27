#ifndef __COMMON_H__
#define __COMMON_H__

#ifndef __IN
#define __IN
#define __OUT
#endif

#define inArea(pos,rect)	((pos.s32X>rect.lTop.s32X&&pos.s32X<rect.rBottom.s32X)&&(pos.s32Y>rect.lTop.s32Y&&pos.s32Y<rect.rBottom.s32Y))
#endif
