#include "window.h"
#include "sw_queue.h"
#include "sw_wmtype.h"
#include "draw.h"

void *contextCreate(void *pWnd_s,void *param);
void *contextEvent(void *pWnd_s,void *param);
void *contextRelease(void *pWnd_s,void *param);

WINDOW_S context_s={
	.hWndId=0,
	.pos_s={0,0,204,0},
	.wintype_e=WIN_CONTEXT,
	.pfOnCreate=contextCreate,
	.pfOnEvent=contextEvent,
	.pfRelease=contextRelease,
	.winSkin_s={0,S_COLOR,0xFFFF},	
	.pWinPrivate=NULL,
};
void *contextCreate(void *pWnd_s,void *param)
{
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

