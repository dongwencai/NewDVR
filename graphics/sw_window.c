#include "window.h"
#include "pthread.h"
#include "sw_queue.h"
#include "sw_fb.h"
#include "windowrep.h"


typedef struct{
	int nWndNum;
	pWINDOWREP_S pWndArray;
}WINREPINFO_S,*pWINREPINFO_S;
static pthread_mutex_t g_winlock;

static pWINDOW_S lookUpWnd(HWND hWndid);
static WINRETSTATUS_E readCtrl(pWINDOW_S pWnd_s);
static WINRETSTATUS_E lookUpWndRep(int nWndId,__OUT pWINDOW_S pWnd_s);
static HWND allocWndId();
static HANDLE allocCtrlId(pWINDOW_S pWnd_s);
int fillRectangle(POINT_S leftTop_s,POINT_S rightBottom_s,U16 u16Color);
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
	if(!pOSD)
	{
		pthread_mutex_init(&g_winlock,NULL);
		createWindow(NULL, OSD, NULL);
	}
	return WIN_WIN_SUC;
}
static WINRETSTATUS_E lookUpWndRep(int nWndId,__OUT pWINDOW_S pWnd_s)
{
	int cnt,nWndNum=sizeof(windowArray)/sizeof(windowArray[0]);
	for(cnt=0;cnt<nWndNum;cnt++)
	{
		if(windowArray[cnt].nwndid==nWndId)
		{
			 *pWnd_s=*windowArray[cnt].wnd_s;
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
	if(!p_Osd)
	{
	
		while(1)
		{
			id=rand()<<16;
			if(!id)	continue;
			return id;
		}
	}
	else
	{
		while(p_Osd)
		{
			
			if(!id)
			{
				id=rand()<<16;
				continue;
			}
			if(p_Osd->winHdl==id)
			{
				id=rand()<<16;
				p_Osd=getOSDWnd();
				continue;
			}
			p_Osd=p_Osd->hWndAbove;
		}
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
				if(temp[cnt].ctrlHdl==id)	continue;
			}
			if(cnt==pWnd_s->winWidget_s.nControlNum)
				break;
			continue;
		}
		return id;
	}
	return 0;
}
void setWndPos(HANDLE hdlWnd,POINT_S pos_s)
{
	pWINDOW_S pWnd_s=lookUpWnd(hdlWnd);
	pWnd_s->pos_s.lTop=pos_s;
	pWnd_s->bRedraw=1;
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
		switch(msg.message)
		{
			case WM_CLOSE:
				closeWindow(pWnd_s->winHdl);
				pthread_exit(NULL);
				break;
			default:
				break;
		}
		msg.message=0;
	}
}
WINRETSTATUS_E createWindow(pWINDOW_S parent,int newWnd,void *param)
{
	pWINDOW_S pNewWnd_s=NULL;
	WINRETSTATUS_E ret;
	pthread_mutex_lock(&g_winlock);
	pNewWnd_s=(pWINDOW_S)malloc(sizeof(WINDOW_S));
	if(!pNewWnd_s)
	{
		WIN_WIN_MEMALLOCFAIL;
	}
	if(ret=lookUpWndRep(newWnd,pNewWnd_s)!=WIN_WIN_SUC)
	{
		return ret;
	}
	pNewWnd_s->winStatus_e=WIN_STATUS_FOCUS;
	pNewWnd_s->hWndId=newWnd;
	pNewWnd_s->winHdl=allocWndId();
	pNewWnd_s->bRedraw=1;
	if(pNewWnd_s->wintype_e==WIN_CONTEXT)
	{
		MS_PARAM ms_param_s=*(pMS_PARAM)param;
		pNewWnd_s->pWinPrivate=&ms_param_s.param;
	}
	if(readCtrl(pNewWnd_s) !=WIN_WIN_SUC)
		return WIN_WIN_FAIL;
	if(pNewWnd_s->pfOnCreate)
	{
		pNewWnd_s->pfOnCreate(pNewWnd_s,param);
	}
	
    if(CreateMsgQueue(&pNewWnd_s->msgid,10)==QUE_SUC)
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
		int ret=0;

		ret=pthread_create(&pNewWnd_s->msgThreadId,NULL,getMsg,pNewWnd_s);
		if(ret)
		{
			if(pNewWnd_s->pfRelease)
			{
				pNewWnd_s->pfRelease(pNewWnd_s,param);
				free(pNewWnd_s);
			}
			return WIN_WIN_FAIL;
		}
	}
	
	if(!parent)
		pOSD=pNewWnd_s;
	else
		parent->hWndAbove=pNewWnd_s;
	
	pNewWnd_s->hWndBottom=parent;
	pNewWnd_s->hWndAbove=NULL;
	pNewWnd_s->hwndChild=NULL;
	pNewWnd_s->hWndParent=NULL;
	pCurWnd=pNewWnd_s;	
	showWindow(pNewWnd_s);
	pthread_mutex_unlock(&g_winlock);
}

WINRETSTATUS_E closeWindow(HANDLE winHdl)
{
	pWINDOW_S pWnd_s=NULL;	
	pthread_mutex_lock(&g_winlock);
	pWnd_s=lookUpWnd(winHdl);
	if(pWnd_s)
	{
		pWnd_s->hWndBottom->hWndAbove=NULL;
		pCurWnd=pWnd_s->hWndBottom;
		pCurWnd->bRedraw=1;
		ReleaseMsgQueue(pWnd_s->msgid);
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
		pthread_mutex_unlock(&g_winlock);
		return WIN_WIN_NOTEXIST;
	}
	//windowFlush();
	pthread_mutex_unlock(&g_winlock);
	return WIN_WIN_SUC;
}
bool winNeedRedraw()
{
	pWINDOW_S ptemp=pOSD;
	pthread_mutex_lock(&g_winlock);
	while(ptemp)
	{
		if(ptemp->bRedraw)
		{
			pthread_mutex_unlock(&g_winlock);
			return 1;
		}
		ptemp = ptemp->hWndAbove;
	}
	pthread_mutex_unlock(&g_winlock);
	return 0;
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
			ptemp->bRedraw=0;
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
	POINT_S lT_s=pWnd->pos_s.lTop,rB_s=pWnd->pos_s.rBottom;
	switch(pWnd->wintype_e)
	{
		case WIN_NORMAL:
		case WIN_CONTEXT:
		case WIN_TOP:
			 drawRectangle(pWnd->pos_s.lTop, pWnd->pos_s.rBottom, pWnd->winColorInfo.u16fg, 1);
			 lT_s.s32X +=1;
			 lT_s.s32Y +=1;
			 fillRectangle(lT_s, rB_s, pWnd->winColorInfo.u16bg);
			 break;
	}
	for(cnt=0;cnt<pWnd->winWidget_s.nControlNum;cnt++)
	{
		if(pWnd->winWidget_s.pControl[cnt].emCtrlStatus !=CTRL_STATUS_INVISABLE)
		{
			showCtrl(&pWnd->winWidget_s.pControl[cnt]);
		}
	}
}
int fillRectangle(POINT_S leftTop_s,POINT_S rightBottom_s,U16 u16Color)
{
	int cnt;
	POINT_S lt=leftTop_s,rb;
	rb.s32X=rightBottom_s.s32X;
	rb.s32Y=lt.s32Y;
	for(cnt=leftTop_s.s32Y;cnt<rightBottom_s.s32Y;cnt++)
	{
		lt.s32Y =cnt;
		rb.s32Y =cnt;
		drawLine(lt,rb,u16Color,1);
	}
}
int showCtrl(pCONTROL pCtrl_s)
{
	POINT_S lT_s=pCtrl_s->pos_s.lTop,rB_s=pCtrl_s->pos_s.rBottom;
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
			drawRectangle(pCtrl_s->pos_s.lTop, pCtrl_s->pos_s.rBottom, pCtrl_s->ctrColorInfo.u16fg, 1);
			lT_s.s32X +=1;
			lT_s.s32Y +=1;
			fillRectangle(lT_s, rB_s, pCtrl_s->ctrColorInfo.u16bg);
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
		if(pWnd_s->wintype_e==WIN_CONTEXT)
		{
			int nCtextId=*(int *)pWnd_s->pWinPrivate;
			lookUpcontextRep(nCtextId,&pWnd_s->winWidget_s);
		}
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

pWINDOW_S posInAboveWnd(POINT_S pos_s)
{
	pWINDOW_S pWnd_s=getCurWnd();
	RECT rect_s;
	while(pWnd_s)
	{
		rect_s=pWnd_s->pos_s;
		if(inArea(pos_s,rect_s))	return pWnd_s;
		pWnd_s = pWnd_s->hWndBottom;
	}
	return NULL;
}
pCONTROL posInCtrl(pWINDOW_S pWnd_s,POINT_S pos_s)
{
	int cnt;
	for(cnt=0;cnt<pWnd_s->winWidget_s.nControlNum;cnt++)
	{
		if(inArea(pos_s, pWnd_s->winWidget_s.pControl[cnt].pos_s))
			return &pWnd_s->winWidget_s.pControl[cnt];
	}
	return NULL;
}
