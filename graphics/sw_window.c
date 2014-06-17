#include "window.h"
#include "pthread.h"
extern gdc;
static pWINDOW_S pOSD=NULL;
static typedef struct{
	int nWndNum;
	pWINDOWREP_S pWndArray;
}WINREPINFO_S,pWINREPINFO_S;

pWINDOW_S getOSDWnd()
{
	return pOSD;
}

int windowInit()
{
	pWINREPINFO_S pWndRepInfo=NULL;
	if(!pOSD)
	{
		pOSD=(WINDOW_S)malloc(sizeof(WINDOW_S));
		memset(pOSD,0x0,sizeof(WINDOW_S));
	}
	if(!pOSD.pWinPrivate)
	{
		extern WINDOWREP_S windowArray;
		pWndRepInfo=(pWINREPINFO_S)malloc(sizeof(WINREPINFO_S));
		if(pWndRepInfo)
		{
			memset(pWndRepInfo,0x0,sizeof(WINREPINFO_S));
			pWndRepInfo.nWndNum=sizeof(windowArray)/windowArray[0];
			pWndRepInfo.pWndArray=windowArray;
			pOSD.pWinPrivate=(pWINREPINFO_S)pWndRepInfo;
		}
		else
		{
			return WIN_WIN_MEMALLOCFAIL;
		}
	}
	pOSD.hWndParent=0;
}
WINRETSTATUS_E lookUpWndRep(int nWndId,__OUT pWINDOW_S pWnd_s)
{
	int cnt;
	pWINREPINFO_S pWndInfo=NULL;
	pWINDOW_S pOsd=getOSDWnd();
	if(!pOsd)
	{
		return WIN_WIN_NOTEXIST;
	}
	pWndInfo=(pWINREPINFO_S)pOsd.pWinPrivate;
	for(cnt=0;cnt<pWndInfo->nWndNum;cnt++)
	{
		if(pWndInfo->pWndArray[cnt]==nWndId)
		{
			 *pWnd_s=pWndInfo->pWndArray[cnt].wnd_s;
			 return WIN_WIN_SUC;
		}
		
	}
	return WIN_WIN_NOTEXIST;
}

pWINDOW_S lookUpWnd(HWND hWndid)
{
	pWINDOW_S temp=NULL;
	temp=getOSDWnd();
	while(temp)
	{
		if(temp->hWndId==hWndid)
		{
			return temp;
		}
		temp=temp->pWINRelationInfo_s.hWndAbove;
	}
	return NULL;
}
int openWindow(pWINDOW_S parent,int newWnd,void *param)
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
	{}
	if(pNewWnd_s->pfOnCreate)
	{
		pNewWnd_s->pfOnCreate(param);
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

		ret=pthread_create(pNewWnd_s->msgThreadId,NULL,pNewWnd_s->pfOnEvent,NULL);
		if(ret)
		{
			if(pNewWnd_s->pfRelease)
			{
				pNewWnd_s->pfRelease(param);
			}
			return WIN_WIN_FAIL;
		}
	}
}

int closeWindow(HWND hWndId)
{
	pWINDOW_S pWnd_s=NULL;	
	pWnd_s=lookUpWnd(hWndId);
	if(!pWnd_s)
	{
		pWnd_s->pWINRelationInfo_s
	}
	else
	{
		return WIN_WIN_NOTEXIST;
	}
	return WIN_WIN_SUC;
}

int openCldWindow()
{

}



int showWindow(pWINDOW_S pWnd)
{

}

pCONTROL lookUpCtrlInWnd(pWINDOW_S pWnd_s,HWND hWndCtrlId)
{
	pCONTROL tmp=NULL;
	if(pWnd_s)
	{
		tmp=pWnd_s->winWidget_s.aControl;
	}
	while(tmp)
	{
		if(tmp->hWnd==hWndCtrlId)
			return tmp;
		tmp=(pCONTROL)tmp->next;		
	}
	return WIN_CTRL_NOTEXIST;
}


