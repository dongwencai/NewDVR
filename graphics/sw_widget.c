#include "window.h"
#include "draw.h"

void *minimizeWnd(void *param);

CONTROL defNormalCtrl_s[4]={
		MK_IMAGE(34,1,56,20,0x83E0,NULL),
		MK_LABLE(4,2,32,18,0x83E0,NULL),	
		MK_BOTTON(26,2,2,18,0x801f,NULL),
		MK_BOTTON(56,2,32,18,0x801f,minimizeWnd),		
};

void *minimizeWnd(void *param)
{
    pMS_PARAM pMs_param=(pMS_PARAM)param;
    if(!pMs_param)  return NULL;
    switch(pMs_param->mesg)
    {
        case WM_LBTN_DOWN:
            setWndHide(pMs_param->pthis);            
            break;
        default:
            break;
    }
    return NULL;
}
