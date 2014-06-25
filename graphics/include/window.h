/////////////////////////////////////////////////////////////////////////////////////head
#ifndef __WINDOW_H__
#define __WINDOW_H__
#include "hi_common.h"
#include "sw_types.h"
#include "bits/pthreadtypes.h"
typedef struct{
	POINT_S lTop;
	POINT_S rBottom;
}RECT,*pRECT;

typedef unsigned long HWND;
typedef unsigned long HANDLE;
typedef void *(*pWinFunc)(void *,void *);
typedef void *(*pWinEvent)(void *);
typedef void *(*pCtrFunc)(void *);

typedef enum{
    LABEL,
    LINE,
    TEXT,
    BUTTON,
    PICTURE,
    CHECK,
    COMBOX,
    SLIDER,
    PROCESS,
    TIMER,
}EMCONTROLTYPE;

typedef enum{
    WIN_NORMAL,
    WIN_CONTEXT,
    WIN_TOP,
    WIN_OSD,
}EMWINTYPE;
typedef enum{
	CTRL_STATUS_NORMAL,
	CTRL_STATUS_DISABLE,
	CTRL_STATUS_INVISABLE,
	CTRL_STATUS_FOCUS,
}EMCTRLSTATUS;
typedef struct{
    bool bLoded;
    int nResId;
    U32 u32ResAddr;
    U32 u32ResLen;
    U16 u16Width;
    U16 u16Height;
}RESINFO;

typedef struct{

        U16 u16fg;
        U16 u16bg;
}COLORINFO;

typedef struct{
    pCtrFunc pfLeftBtnDown;
    pCtrFunc pfLeftBtnUp;
    pCtrFunc pfRightBtnDown;
    pCtrFunc pfRightBtnUp;
    pCtrFunc pfDoubleClick;
    pCtrFunc pfMoveIn;
    pCtrFunc pfMoveOut;
    pCtrFunc pfScroll;
}CTREVENT,*pCTREVENT;

typedef struct{
    HANDLE ctrlHdl;
    RECT pos_s;
    EMCONTROLTYPE emCtrType;
	EMCTRLSTATUS emCtrlStatus;
    RESINFO ctrRes;
    COLORINFO ctrColorInfo;
    pCTREVENT ctrEvent;
    U32   u32Value;
    bool  bRedraw;
}CONTROL,*pCONTROL;

typedef struct{
    pCONTROL pControl;
    int nControlNum;
}WIDGET_S,*p_WIDGET_S;

typedef enum{
	WIN_STATUS_HIDE,
	WIN_STATUS_FOCUS,
	WIN_STATUS_VISIABLE,
	WIN_STATUS_DISABLE,
	WIN_STATUS_NORMAL,
}EMWINSTATUS;

typedef struct WND{
    HWND hWndId;
	HANDLE winHdl;
	pthread_t msgThreadId;
	int  msgid;
	struct WND *hWndParent;
	struct WND *hwndChild;
	struct WND *hWndAbove;
	struct WND *hWndBottom;
    RECT pos_s;
    EMWINTYPE wintype_e;
    EMWINSTATUS winStatus_e;
    RESINFO winRes_s;
	U8 *szctrlRes;
    bool    bRedraw;
	pWinFunc pfOnCreate;
    pWinEvent pfOnEvent;
	pWinFunc pfRelease;
    WIDGET_S winWidget_s;
    void *pWinPrivate;
}WINDOW_S,*pWINDOW_S;

typedef enum{
	WIN_WIN_SUC,
	WIN_WIN_FAIL,
	WIN_WIN_NOTEXIST,
	WIN_WIN_MSG_CREATE_FAIL,
	WIN_WIN_MEMALLOCFAIL,
	WIN_CTRL_NOTEXIST,
}WINRETSTATUS_E;
pWINDOW_S getCurWnd();
pWINDOW_S getOSDWnd();
WINRETSTATUS_E windowInit();
WINRETSTATUS_E createWindow(pWINDOW_S parent,int newWnd,void *param);
WINRETSTATUS_E closeWindow(HWND hWndId);
void windowFlush();
#endif

////////////////////////////////////////////////////////////////////////////////////////////

