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

#define inArea(pos,rect)	((pos.s32X>rect.lTop.s32X&&pos.s32X<rect.rBottom.s32X)&&(pos.s32Y>rect.lTop.s32Y&&pos.s32Y<rect.rBottom.s32Y))

typedef enum{
	OSD,
	CONTEXT,
	SETTING,
	WIN_NUM
}EMWIN;

typedef unsigned long HWND;
typedef unsigned long HANDLE;
typedef void *(*pWinFunc)(void *,void *);
typedef void *(*pCtrFunc)(void *);

typedef enum{
    LABEL,
	IMAGE,
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
	CTRL_STATUS_FOCUS,
	CTRL_STATUS_DISABLE,
	CTRL_STATUS_CLICKED,
	CTRL_STATUS_INVISABLE,
}EMCTRLSTATUS;
typedef struct{
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
typedef void (*WM_CALLBACK)( void* pMsg);
typedef struct{
    pCtrFunc pfLeftBtnDown;
    pCtrFunc pfLeftBtnUp;
    pCtrFunc pfRightBtnDown;
    pCtrFunc pfRightBtnUp;
    pCtrFunc pfDoubleClick;
    pCtrFunc pfMoveIn;
    pCtrFunc pfMoveOut;
    pCtrFunc pfScroll;
}EVENT,*pEVENT;
typedef enum {
	S_IMAGE,
	S_COLOR,
}EMSKIN;
typedef struct{
	int id;
	EMSKIN type;
	union{
		U16 bg;	
		RESINFO res;
	}data;
}SKIN_S;

typedef struct{
    HANDLE ctrlHdl;
    RECT pos_s;
    EMCONTROLTYPE emCtrType;
	EMCTRLSTATUS emCtrlStatus;
    SKIN_S ctrlSkin_s[4];
	WM_CALLBACK cb;
    U32   u32Value;
    bool  bRedraw;
	bool  bCanbefocus;
}CONTROL,*pCONTROL;

typedef struct{
    pCONTROL pControl;
    int nControlNum;
}WIDGET_S,*pWIDGET_S;

typedef enum{
	WIN_STATUS_NORMAL,
	WIN_STATUS_FOCUS,
	WIN_STATUS_DISABLE,
	WIN_STATUS_VISIABLE,
	WIN_STATUS_HIDE,

}EMWINSTATUS;

typedef struct WND{
    HWND hWndId;
	HANDLE winHdl;
	HANDLE focusCtrlHdl;
	pthread_t msgThreadId;
	int  msgid;
	struct WND *hWndParent;
	struct WND *hwndChild;
	struct WND *hWndAbove;
	struct WND *hWndBottom;
    RECT pos_s;
    EMWINTYPE wintype_e;
    EMWINSTATUS winStatus_e;
	SKIN_S	winSkin_s;
	U8 *szctrlRes;
	U32 u32ContextId;
    bool    bRedraw;
	pWinFunc pfOnCreate;
    pWinFunc pfOnEvent;
	pWinFunc pfRelease;
    WIDGET_S winWidget_s;
	EVENT	winEvent;
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

typedef enum{
	WIN_FLASH_ALL,
	WIN_WIN_FLASH,
	WIN_FLASH_AREA,
	WIN_CTRL_FLASH,
}FLASHMSG;

typedef enum{
	WM_LBTN_DOWN,
	WM_LBTN_UP,
	WM_RBTN_DOWN,
	WM_RBTN_UP,
	WM_MBTN_DOWN,
	WM_MBTN_UP,
	WM_MOUSE_MOVE,
	WM_SCROLL,
	WM_CLOSE,
}MS_MSG;

typedef enum{
	NULL_CONTEXT,
	OSD_CONTEXT,
	WND_CONTEXT,
}CONTEXTID;
typedef struct{
	POINT_S pos_s;
	int mesg;
	U32 param;
	pWINDOW_S pthis;
}MS_PARAM,*pMS_PARAM;

pWINDOW_S getCurWnd();
pWINDOW_S getOSDWnd();
WINRETSTATUS_E windowInit();
WINRETSTATUS_E createWindow(pWINDOW_S parent,int newWnd,void *param);
WINRETSTATUS_E closeWindow(HANDLE hWndId);
pWINDOW_S posInAboveWnd(POINT_S pos_s);
pCONTROL posInCtrl(pWINDOW_S pWnd_s,POINT_S pos_s);
void windowFlush();
bool winNeedRedraw();

WINRETSTATUS_E setCtrlLostFocus(pWINDOW_S pWnd_s,pCONTROL pCtrl);
pCONTROL lookUpCtrlInWnd(pWINDOW_S pWnd_s,HANDLE ctrlHdl);
WINRETSTATUS_E setCtrlFocus(pWINDOW_S pWnd_s,pCONTROL pCtrl);
int fillRectangle(POINT_S leftTop_s,POINT_S rightBottom_s,U16 u16Color);
#endif

////////////////////////////////////////////////////////////////////////////////////////////

