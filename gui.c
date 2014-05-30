/////////////////////////////////////////////////////////////////////////////////////head
typedef struct{
    int x;
    int y;
    int width;
    int height;
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
    WIN_HIDE,
    WIN_TOP,
}EMWINTYPE;

typedef struct{
    bool bLoded;
    int nResId;
    U32 u32ResAddr;
    U32 u32ResLen;
    int nResWidth;
    int nResHeight;
}RESINFO;

typedef struct{
    EMCOLORFORMAL emFormal;
    union{
        U16 u16Color[2];
        U32 u32Color[2];
    }COLORS;
}COLORINFO;
typedef struct{
    pCtrFunc LeftBtnDown;
    pCtrFunc LeftBtnUp;
    pCtrFunc RightBtnDown;
    pCtrFunc RightBtnUp;
    pCtrFunc DoubleClick;
    pCtrFunc MoveIn;
    pCtrFunc MoveOut;
    pCtrFunc Scroll;
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
}WIDGET;

typedef struct{
    HWND hWndParent;
    HANDLE hlnstance;
    RECT pos;
    EMWINTYPE wintype;
    EMWINSTATUS winStatus;
    RESINFO winRes;    
    bool    bRedraw;
    pWinFunc CreateWindow;
    pWinFunc OnFocus;
    pWinFunc OnLostFocus;
    pWinFunc OnEvent;
    pWinFunc OnRelease;
    WIDGET  winWidget;
    void *pWinPrivate;
}WINDOW;


////////////////////////////////////////////////////////////////////////////////////////////
