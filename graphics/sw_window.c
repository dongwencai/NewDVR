#include "window.h"

extern gdc;
int windowInit()
{

}
int lookupwnd(int nWndId,__OUT pWINDOW_S pWnd_s)
{
	int cnt;
	extern WINDOWREP_S windowArray;
	for(cnt=0;cnt<sizeof(windowArray)/typeof(windowArray);cnt++)
	{
		if(windowArray[cnt]==nWndId)
		{
			 *pWnd_s=windowArray[cnt].wnd_s;
			 return WIN_SUC;
		}
		
	}
	return WIN_WIN_NOTEXIST;
}
int openwindow(pWINDOW_S parent,int newWnd,void *param)
{
	WINDOW_S newWnd_s={0};
	if(lookupwnd(newWnd,&newWnd_s)!=WIN_SUC)
	{
		
	}
}

int closewindow(HWND)
{
	
}
