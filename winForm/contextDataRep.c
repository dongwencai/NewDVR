#include "window.h"
#include "draw.h"

void botton1_callBack(void *pMsg);
static CONTROL osdCtrlContext[]={
	MK_BOTTON(2, 2, 200, 22, 0x801f, botton1_callBack),
	MK_LINE(1, 26, 204, 26, 0x801f, 2,NULL),
	MK_BOTTON(2, 30, 200, 52, 0x801f, NULL),
	MK_LINE(1, 56, 204, 56, 0x801f, 2,NULL),
};
WIDGET_S osdCtext={osdCtrlContext,sizeof(osdCtrlContext)/sizeof(osdCtrlContext[0])};

typedef struct{
	CONTEXTID id;
	pWIDGET_S  pWidget_s;
	
}CONTEXTREP,*pCONTEXTREP;

CONTEXTREP contextRep[]={
		{NULL_CONTEXT,NULL},
	{OSD_CONTEXT,&osdCtext},
};

int lookUpcontextRep(int nctextid,pWIDGET_S pWidget)
{
	int cnt,nCtextNum=sizeof(contextRep)/sizeof(contextRep[0]);
	for(cnt=0;cnt<nCtextNum;cnt++)
	{
		if(contextRep[cnt].id==nctextid)
		{
			 *pWidget=*contextRep[cnt].pWidget_s;
		}
	}
	return 0;
}

void botton1_callBack(void *pMsg)
{
	pMS_PARAM pMsMsg=(pMS_PARAM)pMsg;
	pWINDOW_S pWnd=pMsMsg->pthis;
	switch(pMsMsg->mesg)
	{
	/*
		case WM_MOUSE_MOVE:
			printf("WM_MOUSE_MOVE\n");
			if(pWnd->winWidget_s.pControl[0].emCtrlStatus==CTRL_STATUS_FOCUS)
				break;
			else
			{
				pWnd->winWidget_s.pControl[0].emCtrlStatus=CTRL_STATUS_FOCUS;
				pWnd->bRedraw=TRUE;
			}
			break;
	*/
	}
}