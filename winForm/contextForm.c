#include "window.h"
#include "sw_queue.h"
#include "sw_wmtype.h"
#include "draw.h"

void *contextCreate(void *pWnd_s,void *param);
void *contextEvent(void *pWnd_s,void *param);
void *contextRelease(void *pWnd_s,void *param);

WINDOW_S context_s={
	.hWndId=0,
	.pos_s={0,0,0,0},
	.wintype_e=WIN_CONTEXT,
	.pfOnCreate=contextCreate,
	.pfOnEvent=contextEvent,
	.pfRelease=contextRelease,
	.winColorInfo={0x801f,0xffff},
	.pWinPrivate=NULL,
};

void *contextCreate(void *pWnd_s,void *param)
{
	int cnt;
	pWINDOW_S pTWnd_s=(pWINDOW_S)pWnd_s;
	MS_PARAM ms_param_s=*(pMS_PARAM)param;
	pTWnd_s->pos_s.lTop.s32X=ms_param_s.pos_s.s32X;
	pTWnd_s->pos_s.lTop.s32Y=ms_param_s.pos_s.s32Y;
	if(pTWnd_s->pos_s.lTop.s32X+204>(getResolution()>>16))
	{
		pTWnd_s->pos_s.lTop.s32X=((getResolution()>>16)-204);
	}
	if(pTWnd_s->pos_s.lTop.s32Y + pTWnd_s->winWidget_s.pControl[pTWnd_s->winWidget_s.nControlNum-1].pos_s.rBottom.s32Y +4>(getResolution()&0xFFFF))
	{
		pTWnd_s->pos_s.lTop.s32Y=(getResolution()&0xFFFF)-pTWnd_s->winWidget_s.pControl[pTWnd_s->winWidget_s.nControlNum-1].pos_s.rBottom.s32Y - 8;
	}
	pTWnd_s->pos_s.rBottom.s32X=pTWnd_s->pos_s.lTop.s32X + 202;
	pTWnd_s->pos_s.rBottom.s32Y=pTWnd_s->pos_s.lTop.s32Y + pTWnd_s->winWidget_s.pControl[pTWnd_s->winWidget_s.nControlNum-1].pos_s.rBottom.s32Y + 3;
	for(cnt=0;cnt<pTWnd_s->winWidget_s.nControlNum;cnt++)
	{
		pTWnd_s->winWidget_s.pControl[cnt].pos_s.lTop.s32X +=pTWnd_s->pos_s.lTop.s32X;
		pTWnd_s->winWidget_s.pControl[cnt].pos_s.lTop.s32Y +=pTWnd_s->pos_s.lTop.s32Y;
		pTWnd_s->winWidget_s.pControl[cnt].pos_s.rBottom.s32X =pTWnd_s->pos_s.rBottom.s32X - pTWnd_s->winWidget_s.pControl[cnt].pos_s.rBottom.s32X;
		pTWnd_s->winWidget_s.pControl[cnt].pos_s.rBottom.s32Y +=pTWnd_s->pos_s.lTop.s32Y;
	}
}
void *contextEvent(void *pWnd_s,void *param)
{
	pWINDOW_S pTWnd_s=(pWINDOW_S)pWnd_s;
	MSG msg=*(pMSG)param;
	switch(msg.message)
	{
		case	WM_WIN_CREATE:
		case	WM_WIN_CLOSE:	
			break;
	}
}
void *contextRelease(void *pWnd_s,void *param)
{
	return HI_NULL;
}

