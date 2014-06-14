/////////////////////////////////////////////////////////////////////////////////////head
typedef struct{
	int x;
	int y;
}POINT_S;
	
typedef struct{
	POINT_S lu;
	POINT_S rb;
}RECT,*pRECT;

typedef unsigned long HWND;
typedef unsigned long HANDLE;

typedef void (*pWinFunc)(void *);
typedef void (*pCtrFunc)(void *);

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
}EMWINTYPE;

typedef struct{
    bool bLoded;
    int nResId;
    U32 u32ResAddr;
    U32 u32ResLen;
    U16 u16Width;
    U16 u16Height;
}RESINFO;

typedef struct{
    EMCOLORFORMAL emFormal;
    union{
        U16 u16Color[2];
        U32 u32Color[2];
    }COLORS;
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
    HWND hwnd;
    RECT pos;
    EMCONTROLTYPE emCtrType;
    RESINFO ctrRes;
    COLORINFO ctrColorInfo;
    pCTREVENT ctrEvent;
    U32   u32Value;
    bool  bRedraw;
}CONTROL,*pCONTROL;

typedef struct{
    CONTROL *aControl;
    int nControlNum;
}WIDGET_S,*p_WIDGET_S;

typedef struct{
	HWND hWndParent;
	HWND hwndChild;
	HWND hWndAbove;
	HWND hWndBottom;
}WIN_RELATION_S,*pWIN_RELATION_S;

typedef enum{
	WIN_HIDE,
	WIN_FOCUS,
	WIN_VISIABLE,
	WIN_DISABLE,
}EMWINSTATUS;

typedef struct{
    HWND hWndParent;
    pWIN_RELATION_S pWINRelationInfo_s;
    RECT pos_s;
    EMWINTYPE wintype_e;
    EMWINSTATUS winStatus_e;
    RESINFO winRes_s;    
    bool    bRedraw;
    pWinFunc pfOnEvent;
    WIDGET_S winWidget_s;
    void *pWinPrivate;
}WINDOW_S;




////////////////////////////////////////////////////////////////////////////////////////////

