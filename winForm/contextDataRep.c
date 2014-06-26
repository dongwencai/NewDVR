#include "window.h"
#include "draw.h"
typedef enum{
	OSD_CONTEXT,
	WND_CONTEXT,
}CONTEXTID;
static CONTROL osdCtrlContext[]={
	MK_BOTTON(2, 2, 4, 22, 0x801f, 0xfc00),
	MK_LINE(1, 26, 0, 26, 0x801f, 2),
	MK_BOTTON(2, 30, 4, 52, 0x801f, 0xfc00),
	MK_LINE(1, 56, 0, 56, 0x801f, 2),
};
WIDGET_S osdCtext={osdCtrlContext,sizeof(osdCtrlContext)/sizeof(osdCtrlContext[0])};

typedef struct{
	CONTEXTID id;
	pWIDGET_S  pWidget_s;
	
}CONTEXTREP,*pCONTEXTREP;

CONTEXTREP contextRep[]={
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
