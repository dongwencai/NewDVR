#ifndef __WINDOWREP_H__
#define __WINDOWREP_H__
#include "window.h"
extern WINDOW_S osd_s;
extern WINDOW_S context_s;
extern WINDOW_S setting_s;


typedef struct{
	int	nwndid;
	pWINDOW_S wnd_s;

}WINDOWREP_S,*pWINDOWREP_S;
	
WINDOWREP_S windowArray[]={
	{OSD,&osd_s},
	{CONTEXT,&context_s},
	{SETTING,&setting_s},
};
#endif
