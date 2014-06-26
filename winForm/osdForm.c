#include "window.h"
#include "sw_queue.h"
#include "sw_wmtype.h"
#include "draw.h"

void *osdCreate(void *pWnd_s,void *param);
void *osdEvent(void *pWnd_s,void *param);
void *osdRelease(void *pWnd_s,void *param);

static CONTROL  osdCtrl[]={
	MK_LINE(600, 0, 600, 720, 0xfc00, 4),
	MK_LINE(0,350,1280,350,0xfc00,4),	
};
WINDOW_S osd_s={
	.hWndId=0,
	.pos_s={0,0,1280,720},
	.wintype_e=WIN_OSD,
	.pfOnCreate=osdCreate,
	.pfOnEvent=osdEvent,
	.pfRelease=osdRelease,
	.winWidget_s={osdCtrl,sizeof(osdCtrl)/sizeof(osdCtrl[0])},
	.pWinPrivate=NULL,
};

void *osdCreate(void *pWnd_s,void *param)
{
	
}
void *osdEvent(void *pWnd_s,void *param)
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
void *osdRelease(void *pWnd_s,void *param)
{
	return HI_NULL;
}
