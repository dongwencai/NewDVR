#include "window.h"
#include "sw_queue.h"
#include "sw_wmtype.h"
#include "draw.h"

void *settingCreate(void *pWnd_s,void *param);
void *settingEvent(void *pWnd_s,void *param);
void *settingRelease(void *pWnd_s,void *param);
static CONTROL  settingCtrl[]={
	MK_BOTTON(50, 50, 120, 70, 0x801f, NULL),
};
WINDOW_S setting_s={
	.hWndId=0,
	.pos_s={150,150,660,440},
	.wintype_e=WIN_NORMAL,
	.pfOnCreate=settingCreate,
	.pfOnEvent=settingEvent,
	.pfRelease=settingRelease,
	.winWidget_s={settingCtrl,sizeof(settingCtrl)/sizeof(settingCtrl[0])},
	.winSkin_s={0,S_COLOR,0xFFFF},
	.u32ContextId=NULL_CONTEXT,
	.pWinPrivate=NULL,
};

void *settingCreate(void *pWnd_s,void *param)
{
	
}
void *settingEvent(void *pWnd_s,void *param)
{
	pWINDOW_S pTWnd_s=(pWINDOW_S)pWnd_s;
	MSG msg=*(pMSG)param;
	pWINDOW_S pCur=getCurWnd();
	MS_PARAM msMsg=*(pMS_PARAM)msg.param;
	/*
	switch(msg.message)
	{
		//case	WM_WIN_CREATE:
		//case	WM_WIN_CLOSE:
		case	WM_RBTN_UP:
			if(pCur->wintype_e==WIN_CONTEXT)
			{
				MSG msg;
				msg.message=WM_CLOSE;
				msg.param=NULL;
				SendMsg(pCur->msgid,msg);
			}
			else
			{
				msMsg.param=1;
				createWindow(pCur, 1, &msMsg);
			}
			break;
	}
	*/
}
void *settingRelease(void *pWnd_s,void *param)
{
	return HI_NULL;
}

