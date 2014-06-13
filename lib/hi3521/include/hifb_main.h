#include "hifb_drv.h"

/* debug print format */
#define HIFB_TRACE(level, fmt...)\
    do{ \
            HI_TRACE(level,HI_ID_FB,"[Func]:%s [Line]:%d [Info]:",__FUNCTION__, __LINE__);\
            HI_TRACE(level,HI_ID_FB,##fmt);\
    }while(0)


#if 1
#define HIFB_FATAL(fmt...)  HIFB_TRACE(HI_DBG_EMERG, fmt)
#define HIFB_ERROR(fmt...)  HIFB_TRACE(HI_DBG_ERR, fmt)
#define HIFB_WARNING(fmt...)  HIFB_TRACE(HI_DBG_WARN, fmt)
#define HIFB_INFO(fmt...)  HIFB_TRACE(HI_DBG_INFO, fmt)
#endif

typedef unsigned char           HI_UCHAR;


/* define the value of default set of each layer */
#define HIFB_HD_DEF_WIDTH    1280     /* unit: pixel */
#define HIFB_HD_DEF_HEIGHT   720     /* unit: pixel */
#define HIFB_HD_DEF_STRIDE   (HIFB_HD_DEF_WIDTH*4)    /* unit: byte */
#define HIFB_HD_DEF_VRAM     7200   /* unit:KB 1280*720*4*2*/

#define HIFB_SD_DEF_WIDTH    720
#define HIFB_SD_DEF_HEIGHT   576
#define HIFB_SD_DEF_STRIDE   1440
#define HIFB_SD_DEF_VRAM     3240   /* unit:KB 720*576*4*/

#define HIFB_AD_DEF_WIDTH    720
#define HIFB_AD_DEF_HEIGHT   576
#define HIFB_AD_DEF_STRIDE   1440
#define HIFB_AD_DEF_VRAM     3240    /*unit:KB 720*576*4*/

#define HIFB_CURSOR_DEF_WIDTH 128
#define HIFB_CURSOR_DEF_HEIGHT 128
#define HIFB_CURSOR_DEF_STRIDE 256
#define HIFB_CURSOR_DEF_VRAM 128    /*unit:KB for doudble buffer mode, we need 2 memory buffer, save cursor*/

#define HIFB_SOFTCURSOR_STRIDE 512

#define HIFB_DEF_DEPTH    16      /* unit: bits */
#define HIFB_DEF_XSTART   0
#define HIFB_DEF_YSTART   0
#define HIFB_DEF_ALPHA    0xff

#define HIFB_CMAP_SIZE  0    /*unit:KB 256*4*/

#define HIFB_IS_CLUTFMT(eFmt)  (HIFB_FMT_1BPP <= (eFmt) && (eFmt) <= HIFB_FMT_ACLUT88)
#define HIFB_ALPHA_OPAQUE    0xff
#define HIFB_ALPHA_TRANSPARENT 0x00

#define HIFB_MIN(m, n) (m) > (n) ? (n) : (m)

#define HIFB_DEFLICKER_LEVEL_MAX 5   /* support level 5 deflicker most */

#define HIFB_MAX_CURSOR_WIDTH 128
#define HIFB_MAX_CURSOR_HEIGHT 128

#define HIFB_MAX_LAYER_ID (HIFB_LAYER_ID_BUTT-1)
#define HIFB_MAX_LAYER_NUM HIFB_LAYER_ID_BUTT

#define HIFB_SPIN_LOCK_FLAG                      unsigned long
#define HIFB_SPIN_LOCK_S                         spinlock_t
#define HIFB_SPIN_LOCK_INIT(pLock)               spin_lock_init(pLock)
#define HIFB_SPIN_LOCK_IRQSAVE(pLock, flag)      spin_lock_irqsave(pLock, flag)
#define HIFB_SPIN_UNLOCK_IRQRESTORE(pLock, flag) spin_unlock_irqrestore(pLock, flag)

typedef enum
{
    HIFB_ANTIFLICKER_NONE,	/* no antiflicker.If scan mode is progressive, hifb will set antiflicker mode to none */
    HIFB_ANTIFLICKER_TDE,	/* tde antiflicker mode, it's effect for 1buf or 2buf only */
    HIFB_ANTIFLICKER_VO,	/* vo antiflicker mode, need hardware supprot */
    HIFB_ANTIFLICKER_BUTT
} HIFB_LAYER_ANTIFLICKER_MODE_E;

typedef struct
{
    HIFB_POINT_S stPos;
    HI_U32 u32DisplayWidth;
    HI_U32 u32DisplayHeight;
    HI_U32 u32ScreenWidth;
    HI_U32 u32ScreenHeight;
    HI_BOOL bPreMul;
    HI_BOOL bNeedAntiflicker;
    HIFB_LAYER_ANTIFLICKER_LEVEL_E enAntiflickerLevel; /* antiflicker level */
    HIFB_LAYER_ANTIFLICKER_MODE_E enAntiflickerMode; /* antiflicker mode */
    HI_U32 u32VirXRes;
    HI_U32 u32VirYRes;
    HI_U32 u32XRes;
    HI_U32 u32YRes;
    HI_U32 u32MaxScreenWidth;
    HI_U32 u32MaxScreenHeight;
}HIFB_DISPLAY_INFO_S;

typedef struct
{
    HI_U32 u32PhyAddr[2];                   /*buf address */
    HI_U32 u32Stride;                       /*buf stride*/
    HI_BOOL bNeedFlip;                      
    HI_BOOL bFliped;                       
    HI_U32 u32IndexForInt;                  
    HI_U32 u32IntPicNum;                    
    HIFB_RECT stUnionRect;                  
    HI_S32 s32RefreshHandle;                
    HI_BOOL bInvalidForVcmp;                
}HIFB_DISPBUF_INFO_S;

typedef struct
{
    HIFB_VCMP_INBUFINFO_S stVcmpInBufInfo;
    HIFB_VCMP_OUTBUFINFO_S astVcmpOutBuf[2];
    HI_U32 u32Width;
    HI_U32 u32Height;
    HI_U32  u32CurOnshowIndex;
    HI_BOOL bVcmpDataReady;
    HI_BOOL bHasVmcpTask;
    HI_U32 u32VcmpTaskNum;  /* PROC info */
}HIFB_VCMPBUF_INFO_S;   

typedef struct 
{
    HIFB_LAYER_BUF_E enBufMode; /* buffer mode */
    HI_BOOL bVcmp;              /* compression enable flag*/
    HI_BOOL bDcmpHasStarted;    
    HIFB_BUFFER_S stUserBuffer;
    HI_U32 u32ScreenAddr;	    /* screen buf addr */
    HIFB_DISPBUF_INFO_S stDispBufInfo;
    HIFB_VCMPBUF_INFO_S stVcmpBufInfo;
    HI_U32 u32RefreshNum;	/* refresh request num in 2 buf mode */
}HIFB_REFRESH_INFO_S;

typedef struct
{
    /*For cursor layer, stCursor means cursor buffer, it is alloced and freed
     both by user;for general layer,stCursor means back buf*/
    HIFB_CURSOR_S stCursor; 

    /*For cursor layer,you can quary whether cursor attach to a certain layer
     for general layer, you can quary whether cursor attach to it*/
    HI_U32 bAttched;

    /*valid area:overlap region between attached layer and cursor layer*/
    HIFB_RECT stRectInDispBuf;

    /*the orignal position of cursor, usually is (0,0) but also has different when at margin*/	
    HIFB_POINT_S stPosInCursor;

    HI_U32 u32AttachedCursorId;
}HIFB_CURSOR_INFO_S;

/* hifb private data*/
typedef struct
{
    HI_U32  u32LayerID;   /* layer id */
    atomic_t ref_count;             /* framebuffer reference count */ 
    HI_BOOL          bShow;         /* show status */
    HIFB_COLOR_FMT_E enColFmt;      /* color format */
    HIFB_ALPHA_S     stAlpha;         /* alpha attribution */
    HIFB_COLORKEYEX_S  stCkey;          /* colorkey attribution */
    HIFB_DISPLAY_INFO_S stDisplayInfo;  /* display info */
    HIFB_REFRESH_INFO_S stRefreshInfo;
    HIFB_CURSOR_INFO_S stCursorInfo;
    HI_U32 u32ParamModifyMask;
    HI_BOOL bModifying;
    HIFB_SURFACE_S CanvasSur;       /* canvas surface */
    HI_U32     u32HDflevel;      /* horizontal deflicker level */
    HI_U32     u32VDflevel;      /* vertical deflicker level */
    HI_UCHAR    ucHDfcoef[HIFB_DEFLICKER_LEVEL_MAX - 1];/* horizontal deflicker coefficients */
    HI_UCHAR    ucVDfcoef[HIFB_DEFLICKER_LEVEL_MAX - 1];/* vertical deflicker coefficients */
    volatile HI_U32 vblflag;
    wait_queue_head_t vblEvent;
    spinlock_t lock;
} HIFB_PAR_S;

typedef struct 
{
    struct fb_info *pstInfo;
    HI_U32 u32LayerSize;     /*u32LayerSize = fb.smem_len, For display buf£¬µ¥Î»ÎªKB*/
    HI_U32 u32CurosrBufSize;    /*For soft cursor*/        
} HIFB_LAYER_S;

typedef enum 
{
    HIFB_LAYER_TYPE_HD,
    HIFB_LAYER_TYPE_SD,
    HIFB_LAYER_TYPE_AD,
    HIFB_LAYER_TYPE_CURSOR,
    HIFB_LAYER_TYPE_BUTT,    
}HIFB_LAYER_TYPE_E;

typedef struct
{
    struct fb_bitfield stRed;     /* bitfield in fb mem if true color, */
    struct fb_bitfield stGreen;   /* else only length is significant */
    struct fb_bitfield stBlue;
    struct fb_bitfield stTransp;  /* transparency	*/
} HIFB_ARGB_BITINFO_S;

typedef struct
{
    HI_U32 u32LayerId;
    HI_BOOL bSoftCursorUpdate;
}HIFB_TDECALLBACK_PARAM;

