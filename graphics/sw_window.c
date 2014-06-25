#include "window.h"
#include "pthread.h"
#include "sw_queue.h"
#include "sw_fb.h"
#include "windowrep.h"


typedef struct{
	int nWndNum;
	pWINDOWREP_S pWndArray;
}WINREPINFO_S,*pWINREPINFO_S;

static pWINDOW_S lookUpWnd(HWND hWndid);
static WINRETSTATUS_E readCtrl(pWINDOW_S pWnd_s);
static WINRETSTATUS_E lookUpWndRep(int nWndId,__OUT pWINDOW_S pWnd_s);
static HWND allocWndId();
static HANDLE allocCtrlId(pWINDOW_S pWnd_s);

extern DC_S gdc;
pWINDOW_S pOSD=NULL;
pWINDOW_S pCurWnd=NULL;

pWINDOW_S getCurWnd()
{
	return pCurWnd;
}
pWINDOW_S getOSDWnd()
{
	return pOSD;
}

WINRETSTATUS_E windowInit()
{
	pWINREPINFO_S pWndRepInfo=NULL;
	if(!pOSD)
	{
		pOSD=(pWINDOW_S)malloc(sizeof(WINDOW_S));
		memcpy(pOSD,&windowArray[0],sizeof(WINDOW_S));
	}
	if(!pOSD->pWinPrivate)
	{
		pWndRepInfo=(pWINREPINFO_S)malloc(sizeof(WINREPINFO_S));
		if(pWndRepInfo)
		{
			memset(pWndRepInfo,0x0,sizeof(WINREPINFO_S));
			pWndRepInfo->nWndNum=sizeof(windowArray)/sizeof(windowArray[0]);
			pWndRepInfo->pWndArray=&windowArray;
			pOSD->pWinPrivate=(pWINREPINFO_S)pWndRepInfo;
		}
		else
		{
			return WIN_WIN_MEMALLOCFAIL;
		}
	}
	pOSD->hWndParent=NULL;
	pOSD->hWndBottom=NULL;
	showWindow(pOSD);
	pCurWnd=pOSD;
	return WIN_WIN_SUC;
}
static WINRETSTATUS_E lookUpWndRep(int nWndId,__OUT pWINDOW_S pWnd_s)
{
	int cnt;
	pWINREPINFO_S pWndInfo=NULL;
	pWINDOW_S pOsd=getOSDWnd();
	if(!pOsd)
	{
		return WIN_WIN_NOTEXIST;
	}
	pWndInfo=(pWINREPINFO_S)pOsd->pWinPrivate;
	for(cnt=0;cnt<pWndInfo->nWndNum;cnt++)
	{
		if(pWndInfo->pWndArray[cnt].nwndid==nWndId)
		{
			 *pWnd_s=*pWndInfo->pWndArray[cnt].wnd_s;
			 return WIN_WIN_SUC;
		}
		
	}
	return WIN_WIN_NOTEXIST;
}

static pWINDOW_S lookUpWnd(HANDLE hWndid)
{
	pWINDOW_S temp=NULL;
	temp=getOSDWnd();
	while(temp)
	{
		if(temp->winHdl==hWndid)
		{
			return temp;
		}
		temp=temp->hWndAbove;
	}
	return NULL;
}
static HANDLE allocWndId()
{
	HANDLE id=0;
	pWINDOW_S p_Osd=getOSDWnd();
	while(p_Osd)
	{
		if((id=(rand()<<16))==0)	continue;
		if(p_Osd->winHdl==id)
		{
			p_Osd=getOSDWnd();
			continue;
		}
		p_Osd=p_Osd->hWndAbove;
	}
	return id;
}
static HANDLE allocCtrlId(pWINDOW_S pWnd_s)
{
	pCONTROL temp=NULL;
	HANDLE	 id=0;
	if(pWnd_s&&pWnd_s->winWidget_s.pControl)
	{
		int cnt;
		temp=pWnd_s->winWidget_s.pControl;
		while(1)
		{
			id=((rand()<<16)>>16)|pWnd_s->winHdl;
			for(cnt=0;cnt<pWnd_s->winWidget_s.nControlNum;cnt++)
			{
				if(temp[cnt].ctrlHdl==id)	break;
			}
			if(cnt==pWnd_s->winWidget_s.nControlNum-1)
				break;
			continue;
		}
		return id;
	}
	return 0;
}

void *getMsg(void *param)
{
	pWINDOW_S pWnd_s=(pWINDOW_S)param;	
	MSG		msg={0};
	while(1)
	{
		if(RecvMsg(pWnd_s->msgid,&msg) !=QUE_SUC)
		{
			continue;
		}
		pWnd_s->pfOnEvent(pWnd_s,&msg);
		if(msg.param)
		{
			free(msg.param);
			msg.param=NULL;
		}
		msg.message=0;
	}
}
WINRETSTATUS_E createWindow(pWINDOW_S parent,int newWnd,void *param)
{
	pWINDOW_S pNewWnd_s=NULL;
	WINRETSTATUS_E ret;
	pNewWnd_s=(pWINDOW_S)malloc(sizeof(WINDOW_S));
	if(!pNewWnd_s)
	{
		WIN_WIN_MEMALLOCFAIL;
	}
	if(ret=lookUpWndRep(newWnd,pNewWnd_s)!=WIN_WIN_SUC)
	{
		return ret;
	}
	pNewWnd_s->winHdl=allocWndId();
printf("%s\t%d\t%d\n",__FUNCTION__,__LINE__,pNewWnd_s->winHdl);
	if(readCtrl(pNewWnd_s) !=WIN_WIN_SUC)
		return WIN_WIN_FAIL;
	if(pNewWnd_s->pfOnCreate)
	{
		pNewWnd_s->pfOnCreate(pNewWnd_s,param);
	}
    if(CreateMsgQueue(pNewWnd_s->msgid,10)==QUE_SUC)
    {
        printf("Queue created success!\n");
    }
    else
    {
        printf("Queue created failt!\n");
        return WIN_WIN_MSG_CREATE_FAIL;
    }
	if(pNewWnd_s->pfOnEvent)
	{
		int ret;

		ret=pthread_create(pNewWnd_s->msgThreadId,NULL,getMsg,pNewWnd_s);
		if(ret)
		{
			if(pNewWnd_s->pfRelease)
			{
				pNewWnd_s->pfRelease(pNewWnd_s,param);
			}
			return WIN_WIN_FAIL;
		}
	}
	pNewWnd_s->hWndBottom=parent;
	pNewWnd_s->hWndAbove=NULL;
	pNewWnd_s->hwndChild=NULL;
	pNewWnd_s->hWndParent=NULL;
	parent->hWndAbove=pNewWnd_s;
	showWindow(pNewWnd_s);
}

WINRETSTATUS_E closeWindow(HWND hWndId)
{
	pWINDOW_S pWnd_s=NULL;	
	pWnd_s=lookUpWnd(hWndId);
	if(!pWnd_s)
	{
		pWnd_s->hWndBottom=NULL;
		if(pWnd_s->pfRelease)
		{
			pWnd_s->pfRelease(pWnd_s,NULL);
		}
		if(pWnd_s->winWidget_s.pControl)
		{
			free(pWnd_s->winWidget_s.pControl);
			free(pWnd_s);
		}
	}
	else
	{
		return WIN_WIN_NOTEXIST;
	}
	windowFlush();
	return WIN_WIN_SUC;
}

void eraseWin()
{
	memset(gdc.pWinFb->pMappedAddr,0x00,gdc.pSzWin.nH*gdc.pSzWin.nW*2);
}

void windowFlush()
{
	pWINDOW_S ptemp=NULL;
	ptemp=getOSDWnd();
	eraseWin();
	while(ptemp)
	{
		if(ptemp->winStatus_e !=WIN_STATUS_HIDE&&ptemp->winStatus_e !=WIN_STATUS_VISIABLE)
		{
			showWindow(ptemp);
		}
		ptemp=ptemp->hWndAbove;
	}
}

int openCldWindow()
{

}



int showWindow(pWINDOW_S pWnd)
{
	int cnt;
	switch(pWnd->wintype_e)
	{
		case WIN_NORMAL:
		case WIN_TOP:
			 drawRectangle(pWnd->pos_s.lTop, pWnd->pos_s.rBottom, 0xfc00, 4);
	}
	for(cnt=0;cnt<pWnd->winWidget_s.nControlNum;cnt++)
	{
		if(pWnd->winWidget_s.pControl[cnt].emCtrlStatus !=CTRL_STATUS_INVISABLE)
			showCtrl(&pWnd->winWidget_s.pControl[cnt]);
	}
}
int showCtrl(pCONTROL pCtrl_s)
{
	switch(pCtrl_s->emCtrType)
	{
		case	LINE:
			drawLine(pCtrl_s->pos_s.lTop,pCtrl_s->pos_s.rBottom,pCtrl_s->ctrColorInfo.u16fg,pCtrl_s->u32Value);
			break;
		case	LABEL:
		case	BUTTON:
		case	PICTURE:
		case	CHECK:
		case	COMBOX:
		case	SLIDER:
		case	PROCESS:
			drawRectangle(pCtrl_s->pos_s.lTop, pCtrl_s->pos_s.rBottom, pCtrl_s->ctrColorInfo.u16fg, 4);
			break;
		default:
			printf("error:ctrl_s.emCtrType\t%d\n",pCtrl_s->emCtrType);
			break;
	}
	return 0;
}
pCONTROL lookUpCtrlInWnd(pWINDOW_S pWnd_s,HWND hWndCtrlId)
{
	pCONTROL tmp=NULL;
	if(pWnd_s&&pWnd_s->winWidget_s.pControl)
	{
		int cnt;
		tmp=pWnd_s->winWidget_s.pControl;
		for(cnt=0;cnt<pWnd_s->winWidget_s.nControlNum;cnt++)
		{
			if(tmp[cnt].ctrlHdl==hWndCtrlId)
				return &tmp[cnt];
		}
	}

	return WIN_CTRL_NOTEXIST;
}

static WINRETSTATUS_E readCtrl(pWINDOW_S pWnd_s)
{
	if(pWnd_s->szctrlRes)
	{
		
	}
	else
	{
		WIDGET_S widget;
		int cnt;
		widget.nControlNum=pWnd_s->winWidget_s.nControlNum;
		widget.pControl=(pCONTROL)malloc(sizeof(pWnd_s->winWidget_s.nControlNum)*sizeof(CONTROL));
		if(!widget.pControl)
		{
			return WIN_WIN_MEMALLOCFAIL;
		}
		memcpy(widget.pControl,pWnd_s->winWidget_s.pControl,sizeof(pWnd_s->winWidget_s.nControlNum)*sizeof(CONTROL));
		for(cnt=0;cnt<widget.nControlNum;cnt++)
		{
			widget.pControl[cnt].ctrlHdl=allocCtrlId(pWnd_s);
		}
		pWnd_s->winWidget_s=widget;
	}
	return WIN_WIN_SUC;
}


