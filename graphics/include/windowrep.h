#ifndef __WINDOWREP_H__
#define __WINDOWREP_H__
typedef enum{
	CONTEXT,
	TASKBAR,
};

typedef struct{
	int	nwndid;
	WINDOW_S wnd_s;

}WINDOWREP_S,pWINDOWREP_S;
	
WINDOWREP_S windowArray[]={
	{CONTEXT,},

};
#endif
