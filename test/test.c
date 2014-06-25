#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/time.h>
#include <sys/mman.h>   //mmap
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <assert.h>
#include <signal.h>

#include "hi_common.h"
#include "hi_type.h"
#include "hi_comm_vb.h"
#include "hi_comm_sys.h"
#include "hi_comm_venc.h"
#include "hi_comm_vi.h"
#include "hi_comm_vo.h"
#include "hi_comm_region.h"

#include "mpi_vb.h"
#include "mpi_sys.h"
#include "mpi_venc.h"
#include "mpi_vi.h"
#include "mpi_vo.h"
#include "mpi_region.h"
#include "draw.h"
#include <linux/fb.h>
#include "hifb.h"



#include "hi_tde_api.h"
#include "hi_tde_type.h"
#include "hi_tde_errcode.h"

#include "sw_fb.h"
#include "sw_types.h"
#include "draw.h"
#define sw_VO_DEV_DHD0 0
#define sw_SYS_ALIGN_WIDTH      64
#define sw_PIXEL_FORMAT         PIXEL_FORMAT_YUV_SEMIPLANAR_420
#define ALIGN_BACK(x, a)              ((a) * (((x) / (a))))

typedef enum sw_vi_mode_e
{   /* For Hi3531 or Hi3532 */
    sw_VI_MODE_1_D1 = 0,
    sw_VI_MODE_16_D1,
    sw_VI_MODE_16_960H,
    sw_VI_MODE_4_720P,
    sw_VI_MODE_4_1080P,
    /* For Hi3521 */
	sw_VI_MODE_8_D1,
	sw_VI_MODE_1_720P,
	sw_VI_MODE_16_Cif,
	sw_VI_MODE_16_2Cif,
	sw_VI_MODE_16_D1Cif,
	sw_VI_MODE_1_D1Cif,
	/*For Hi3520A*/
    
    sw_VI_MODE_4_D1,
    sw_VI_MODE_8_2Cif,
    /*For Hi3520D*/
    sw_VI_MODE_1_1080P,
    sw_VI_MODE_8_D1Cif,
}sw_VI_MODE_E;
typedef enum sw_vo_mode_e
{
    VO_MODE_1MUX  = 0,
    VO_MODE_4MUX = 1,
    VO_MODE_9MUX = 2,
    VO_MODE_16MUX = 3,
    VO_MODE_BUTT,
}sw_VO_MODE_E;

static VO_DEV VoDev = sw_VO_DEV_DHD0;
DC_S gdc;

void sw_VIO_HandleSig(HI_S32 signo)
{
    if (SIGINT == signo || SIGTSTP == signo)
    {
        sw_COMM_SYS_Exit();
        printf("\033[0;31mprogram exit abnormally!\033[0;39m\n");
    }

    exit(0);
}


HI_S32 sw_HIFB_VO_Start(void)
{
#define HIFB_HD_WIDTH  1280
#define HIFB_HD_HEIGHT 720

    HI_S32 s32Ret = HI_SUCCESS;

    VO_PUB_ATTR_S stPubAttr;
    VO_VIDEO_LAYER_ATTR_S stLayerAttr;
    VO_CHN_ATTR_S stChnAttr;

    HI_MPI_VO_Disable(VoDev);

    stPubAttr.enIntfType = VO_INTF_VGA;
    stPubAttr.enIntfSync = VO_OUTPUT_720P50;
    stPubAttr.u32BgColor = 0xff0000ff;

    stLayerAttr.stDispRect.s32X       = 0;
    stLayerAttr.stDispRect.s32Y       = 0;
    stLayerAttr.stDispRect.u32Width   = HIFB_HD_WIDTH;
    stLayerAttr.stDispRect.u32Height  = HIFB_HD_HEIGHT;
    stLayerAttr.stImageSize.u32Width  = HIFB_HD_WIDTH;
    stLayerAttr.stImageSize.u32Height = HIFB_HD_HEIGHT;
    stLayerAttr.u32DispFrmRt          = 50;
    stLayerAttr.enPixFormat           = PIXEL_FORMAT_YUV_SEMIPLANAR_422;

    stChnAttr.stRect.s32X               = 0;
    stChnAttr.stRect.s32Y               = 0;
    stChnAttr.stRect.u32Width           = HIFB_HD_WIDTH;
    stChnAttr.stRect.u32Height          = HIFB_HD_HEIGHT;
    stChnAttr.bDeflicker                = HI_FALSE;
    stChnAttr.u32Priority               = 1;

    if (HI_SUCCESS != HI_MPI_VO_SetPubAttr(VoDev, &stPubAttr))
    {
        printf("set VO pub attr failed !\n");
        return -1;
    }

    if (HI_SUCCESS != HI_MPI_VO_Enable(VoDev))
    {
        printf("enable vo device failed!\n");
        return -1;
    }

    s32Ret = HI_MPI_VO_SetVideoLayerAttr(VoDev, &stLayerAttr);
    if (s32Ret != HI_SUCCESS)
    {
        printf("set video layer attr failed with %#x!\n", s32Ret);
        return -1;
    }

    if (HI_SUCCESS != HI_MPI_VO_EnableVideoLayer(VoDev))
    {
        printf("enable video layer failed!\n");
        return -1;
    }

    return 0;
}

HI_S32 sw_HIFB_VO_Stop(void)
{
    if (HI_SUCCESS != HI_MPI_VO_DisableVideoLayer(VoDev))
    {
        printf("Disable video layer failed!\n");
        return -1;
    }

    if (HI_SUCCESS != HI_MPI_VO_Disable(VoDev))
    {
        printf("Disable vo device failed!\n");
        return -1;
    }

    return 0;
}

#define sw_IMAGE_WIDTH     300
#define sw_IMAGE_HEIGHT    150
#define sw_IMAGE_SIZE      (300*150*2)
#define sw_IMAGE_NUM       20

#define sw_IMAGE_PATH		"./res/%d.bmp"
#define sw_CURSOR_PATH		"./res/cursor.bmp"

#define DIF_LAYER_NAME_LEN 20
#define HIL_MMZ_NAME_LEN 32
#define HIFB_RED_1555   0xfc00
#define sw_VIR_SCREEN_WIDTH	    sw_IMAGE_WIDTH			/*virtual screen width*/
#define sw_VIR_SCREEN_HEIGHT	sw_IMAGE_HEIGHT*2		/*virtual screen height*/
#define s32fd 0
#define HIL_MMB_NAME_LEN 16
#define g_s32fd  0

typedef enum
{
    HIFB_LAYER_0 = 0x0,
    HIFB_LAYER_1,
    HIFB_LAYER_2,
    HIFB_LAYER_CURSOR_0,
    HIFB_LAYER_ID_BUTT
} HIFB_LAYER_ID_E;

typedef struct _LayerID_NAME_S
{
    HIFB_LAYER_ID_E     sLayerID;
    HI_CHAR             sLayerName[DIF_LAYER_NAME_LEN];
} LayerID_NAME_S;

typedef struct hiPTHREAD_HIFB_sw
{
    int fd;
    int layer;
    int ctrlkey;
} PTHREAD_HIFB_sw_INFO;



HI_VOID *sw_HIFB_REFRESH(void *pData)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HIFB_LAYER_INFO_S stLayerInfo = {0};
    HIFB_BUFFER_S stCanvasBuf;
    HI_U16 *pBuf;
    HI_U32 x, y;
    HI_BOOL Show;
    HI_BOOL bCompress = HI_TRUE;
    HIFB_POINT_S stPoint = {0};
    struct fb_var_screeninfo stVarInfo;
    char file[12] = "/dev/fb0";
    HI_U32 maxW,maxH;
    PTHREAD_HIFB_sw_INFO *pstInfo;
    pstInfo = (PTHREAD_HIFB_sw_INFO *)pData;
    HIFB_COLORKEY_S stColorKey;
    switch (pstInfo->layer)
    {
    case 0 :
        strcpy(file, "/dev/fb0");
        break;
    case 1 :
        strcpy(file, "/dev/fb1");
        break;
    case 2 :
        strcpy(file, "/dev/fb2");
        break;
    case 3 :
        strcpy(file, "/dev/fb3");
        break;
    case 4 :
        strcpy(file, "/dev/fb4");
        break;

    default:
        strcpy(file, "/dev/fb0");
        break;
    }

    pstInfo->fd = open(file, O_RDWR, 0);
    if (pstInfo->fd < 0)
    {
        printf("open %s failed!\n",file);
        return HI_NULL;
    }

    if (pstInfo->layer == HIFB_LAYER_0 )
    {
        if (ioctl(pstInfo->fd, FBIOPUT_COMPRESSION_HIFB, &bCompress) < 0)
        {
            printf("FBIOPUT_COMPRESSION_HIFB failed!\n");
            close(pstInfo->fd);
            return HI_NULL;
        }
    }
    stColorKey.bKeyEnable = HI_TRUE;
    stColorKey.u32Key = 0x0;
    if (ioctl(pstInfo->fd, FBIOPUT_COLORKEY_HIFB, &stColorKey) < 0)
    {
        printf("FBIOPUT_COLORKEY_HIFB!\n");
        close(pstInfo->fd);
        return HI_NULL;
    }
    s32Ret = ioctl(pstInfo->fd, FBIOGET_VSCREENINFO, &stVarInfo);
    if (s32Ret < 0)
    {
        printf("GET_VSCREENINFO failed!\n");
        return HI_NULL;
    }

    if (ioctl(pstInfo->fd, FBIOPUT_SCREEN_ORIGIN_HIFB, &stPoint) < 0)
    {
        printf("set screen original show position failed!\n");
        return HI_NULL;
    }
    maxW = 1280;
    maxH = 720;
    stVarInfo.xres = stVarInfo.xres_virtual = maxW;
    stVarInfo.yres = stVarInfo.yres_virtual = maxH;
    s32Ret = ioctl(pstInfo->fd, FBIOPUT_VSCREENINFO, &stVarInfo);
    if (s32Ret < 0)
    {
        printf("PUT_VSCREENINFO failed!\n");
        return HI_NULL;
    }
    switch (pstInfo->ctrlkey)
    {
    case 0 :
    {
        stLayerInfo.BufMode = HIFB_LAYER_BUF_ONE;
        stLayerInfo.u32Mask = HIFB_LAYERMASK_BUFMODE;
        break;
    }

    case 1 :
    {
        stLayerInfo.BufMode = HIFB_LAYER_BUF_DOUBLE;
        stLayerInfo.u32Mask = HIFB_LAYERMASK_BUFMODE;
        break;
    }

    default:
    {
        stLayerInfo.BufMode = HIFB_LAYER_BUF_NONE;
        stLayerInfo.u32Mask = HIFB_LAYERMASK_BUFMODE;
    }
    }
    s32Ret = ioctl(pstInfo->fd, FBIOPUT_LAYER_INFO, &stLayerInfo);
    if (s32Ret < 0)
    {
        printf("PUT_LAYER_INFO failed!\n");
        return HI_NULL;
    }
    Show = HI_TRUE;
    if (ioctl(pstInfo->fd, FBIOPUT_SHOW_HIFB, &Show) < 0)
    {
        printf("FBIOPUT_SHOW_HIFB failed!\n");
        return HI_NULL;
    }
    if (HI_FAILURE == HI_MPI_SYS_MmzAlloc(&(stCanvasBuf.stCanvas.u32PhyAddr), ((void**)&pBuf),
                                          NULL, NULL, maxW*maxH*2))
    {
        printf("allocate memory (maxW*maxH*2 bytes) failed\n");
        return HI_NULL;
    }
    stCanvasBuf.stCanvas.u32Height = maxH;
    stCanvasBuf.stCanvas.u32Width = maxW;
    stCanvasBuf.stCanvas.u32Pitch = maxW*2;
    stCanvasBuf.stCanvas.enFmt = HIFB_FMT_ARGB1555;
    memset(pBuf, 0x00, stCanvasBuf.stCanvas.u32Pitch*stCanvasBuf.stCanvas.u32Height);
	gdc.tcClear=0x00;
	gdc.pWinFb->hFB=pstInfo->fd;
	gdc.pWinFb->pMappedAddr=pBuf;
	gdc.pWinFb->pPhyAddr=stCanvasBuf.stCanvas.u32PhyAddr;
	gdc.pSzWin.nW=maxW;
	gdc.pSzWin.nH=maxH;

    {
		
		{
			POINT_S begin_s,end_s;
			begin_s.s32X=60;begin_s.s32Y=60;
			end_s.s32X=160;end_s.s32Y=160;
			drawRectangle(begin_s,end_s,HIFB_RED_1555,2);
    	}

        stCanvasBuf.UpdateRect.x = 0;
        stCanvasBuf.UpdateRect.y = 0;
        stCanvasBuf.UpdateRect.w = maxW;
        stCanvasBuf.UpdateRect.h = maxH;
        s32Ret = ioctl(pstInfo->fd, FBIO_REFRESH, &stCanvasBuf);
        if (s32Ret < 0)
        {
            printf("REFRESH failed!\n");
            return HI_NULL;
        }

    }
	while(1);
    close(pstInfo->fd);
    return HI_NULL;
}
HI_S32 sw_COMM_SYS_Init(VB_CONF_S *pstVbConf)
{
    MPP_SYS_CONF_S stSysConf = {0};
    HI_S32 s32Ret = HI_FAILURE;

    HI_MPI_SYS_Exit();
    HI_MPI_VB_Exit();

    if (NULL == pstVbConf)
    {
        printf("input parameter is null, it is invaild!\n");
        return HI_FAILURE;
    }

    s32Ret = HI_MPI_VB_SetConf(pstVbConf);
    if (HI_SUCCESS != s32Ret)
    {
        printf("HI_MPI_VB_SetConf failed!\n");
        return HI_FAILURE;
    }

    s32Ret = HI_MPI_VB_Init();
    if (HI_SUCCESS != s32Ret)
    {
        printf("HI_MPI_VB_Init failed!\n");
        return HI_FAILURE;
    }

    stSysConf.u32AlignWidth = sw_SYS_ALIGN_WIDTH;
    s32Ret = HI_MPI_SYS_SetConf(&stSysConf);
    if (HI_SUCCESS != s32Ret)
    {
        printf("HI_MPI_SYS_SetConf failed\n");
        return HI_FAILURE;
    }

    s32Ret = HI_MPI_SYS_Init();
    if (HI_SUCCESS != s32Ret)
    {
        printf("HI_MPI_SYS_Init failed!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 sw_COMM_VO_StartDevLayer(VO_DEV VoDev, VO_PUB_ATTR_S *pstPubAttr, HI_U32 u32SrcFrmRate)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32Width = 0;
    HI_U32 u32Height = 0;
    HI_U32 u32Frm = 0;
    VO_VIDEO_LAYER_ATTR_S stLayerAttr;

    if ( 0 == u32SrcFrmRate )
    {
        printf("vo u32SrcFrmRate invaild! %d!\n", u32SrcFrmRate);
        return HI_FAILURE;
    }
    s32Ret = HI_MPI_VO_SetPubAttr(VoDev, pstPubAttr);
    if (s32Ret != HI_SUCCESS)
    {
        printf("failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }

    s32Ret = HI_MPI_VO_Enable(VoDev);
    if (s32Ret != HI_SUCCESS)
    {
        printf("failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }

    s32Ret = sw_COMM_VO_GetWH(pstPubAttr->enIntfSync, &u32Width, &u32Height, &u32Frm);
    if (s32Ret != HI_SUCCESS)
    {
        printf("failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }

    stLayerAttr.enPixFormat = sw_PIXEL_FORMAT;
    stLayerAttr.u32DispFrmRt = u32SrcFrmRate;

    stLayerAttr.stDispRect.s32X       = 0;
    stLayerAttr.stDispRect.s32Y       = 0;
    stLayerAttr.stDispRect.u32Width   = u32Width;
    stLayerAttr.stDispRect.u32Height  = u32Height;
    stLayerAttr.stImageSize.u32Width  = u32Width;
    stLayerAttr.stImageSize.u32Height = u32Height;
    
    s32Ret = HI_MPI_VO_SetVideoLayerAttr(VoDev, &stLayerAttr);
    if (s32Ret != HI_SUCCESS)
    {
        printf("failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }

    s32Ret = HI_MPI_VO_EnableVideoLayer(VoDev);
    if (s32Ret != HI_SUCCESS)
    {
        printf("failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }
    
    return s32Ret;
}

HI_VOID sw_COMM_SYS_Exit(void)
{
    HI_MPI_SYS_Exit();
    HI_MPI_VB_Exit();
    return;
}

HI_S32 sw_COMM_VO_GetWH(VO_INTF_SYNC_E enIntfSync, HI_U32 *pu32W,HI_U32 *pu32H, HI_U32 *pu32Frm)
{
    switch (enIntfSync)
    {
        case VO_OUTPUT_PAL       :  *pu32W = 720;  *pu32H = 576; *pu32Frm = 25; break;
        case VO_OUTPUT_NTSC      :  *pu32W = 720;  *pu32H = 480; *pu32Frm = 30; break;
        case VO_OUTPUT_800x600_60:  *pu32W = 800;  *pu32H = 600;  *pu32Frm = 60; break;
        case VO_OUTPUT_720P50    :  *pu32W = 1280; *pu32H = 720;  *pu32Frm = 50; break;
        case VO_OUTPUT_1080P24  : *pu32W = 1920; *pu32H = 1080;  *pu32Frm = 24; break;
        case VO_OUTPUT_720P60    : *pu32W = 1280; *pu32H = 720;  *pu32Frm = 60; break;
        case VO_OUTPUT_1080P30   :  *pu32W = 1920; *pu32H = 1080; *pu32Frm = 30; break;
        case VO_OUTPUT_1080P25   :  *pu32W = 1920; *pu32H = 1080; *pu32Frm = 25; break;
        case VO_OUTPUT_1080P50   :  *pu32W = 1920; *pu32H = 1080; *pu32Frm = 50; break;
        case VO_OUTPUT_1080P60   :  *pu32W = 1920; *pu32H = 1080; *pu32Frm = 60; break;
        case VO_OUTPUT_1024x768_60:  *pu32W = 1024; *pu32H = 768;  *pu32Frm = 60; break;
        case VO_OUTPUT_1280x1024_60:  *pu32W = 1280; *pu32H = 1024;  *pu32Frm = 60; break;
        case VO_OUTPUT_1366x768_60:   *pu32W = 1366; *pu32H = 768;  *pu32Frm = 60; break;
        case VO_OUTPUT_1440x900_60: *pu32W = 1440; *pu32H = 900;  *pu32Frm = 60; break;
        case VO_OUTPUT_1280x800_60: *pu32W = 1280; *pu32H = 800;  *pu32Frm = 60; break;
        default: 
            printf("vo enIntfSync not support!\n");
            return HI_FAILURE;
    }
    return HI_SUCCESS;
}
HI_S32 sw_COMM_VO_StartChn(VO_DEV VoDev,VO_PUB_ATTR_S *pstPubAttr,sw_VO_MODE_E enMode)
{
    HI_S32 i;
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32WndNum = 0;
    HI_U32 u32Square = 0;
    HI_U32 u32Width = 0;
    HI_U32 u32Height = 0;
    HI_U32 u32Frm = 0;
    VO_CHN_ATTR_S stChnAttr;
    
    switch (enMode)
    {
        case VO_MODE_1MUX:
            u32WndNum = 1;
            u32Square = 1;
            break;
        case VO_MODE_4MUX:
            u32WndNum = 4;
            u32Square = 2;
            break;
        case VO_MODE_9MUX:
            u32WndNum = 9;
            u32Square = 3;
            break;
        case VO_MODE_16MUX:
            u32WndNum = 16;
            u32Square = 4;
            break;
        default:
            printf("failed with %#x!\n", s32Ret);
            return HI_FAILURE;
    }

    s32Ret = sw_COMM_VO_GetWH(pstPubAttr->enIntfSync, &u32Width,&u32Height,&u32Frm);
    if (s32Ret != HI_SUCCESS)
    {
        printf("failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }
    for (i=0; i<u32WndNum; i++)
    {
        stChnAttr.stRect.s32X       = ALIGN_BACK((u32Width/u32Square) * (i%u32Square), 2);
        stChnAttr.stRect.s32Y       = ALIGN_BACK((u32Height/u32Square) * (i/u32Square), 2);
        stChnAttr.stRect.u32Width   = ALIGN_BACK(u32Width/u32Square, 2);
        stChnAttr.stRect.u32Height  = ALIGN_BACK(u32Height/u32Square, 2);
        stChnAttr.u32Priority       = 0;
        stChnAttr.bDeflicker        = HI_FALSE;
		
        s32Ret = HI_MPI_VO_SetChnAttr(VoDev, i, &stChnAttr);
        if (s32Ret != HI_SUCCESS)
        {
            printf("%s(%d):failed with %#x!\n",\
                   __FUNCTION__,__LINE__,  s32Ret);
            return HI_FAILURE;
        }

        s32Ret = HI_MPI_VO_EnableChn(VoDev, i);
        if (s32Ret != HI_SUCCESS)
        {
            printf("failed with %#x!\n", s32Ret);
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}
initFb()
{
	gdc.pWinFb=(PFRAMEBUFFER)malloc(sizeof(FRAMEBUFFER));
}
int main(int argc, char *argv[])
{
    pthread_t phifb0 = -1;
    PTHREAD_HIFB_sw_INFO stInfo0;
    PTHREAD_HIFB_sw_INFO stInfo1;
    VO_PUB_ATTR_S stPubAttr;
    VB_CONF_S stVbConf;
    HI_S32 s32Ret = HI_SUCCESS;
    HI_S32 i;
    sw_VI_MODE_E enViMode = sw_VI_MODE_1_D1;
    sw_VO_MODE_E stVoMode = VO_MODE_1MUX;
    HI_BOOL bExtendedMode=HI_TRUE;
    HI_CHAR ch;
initFb();
    memset(&stVbConf, 0, sizeof(VB_CONF_S));
    stVbConf.u32MaxPoolCnt             = 16;
    stVbConf.astCommPool[0].u32BlkSize = 720*576*2;
    stVbConf.astCommPool[0].u32BlkCnt  = 16;

    stPubAttr.u32BgColor = 0xff00ff00;
    stPubAttr.enIntfType = VO_INTF_VGA;
    stPubAttr.enIntfSync = VO_OUTPUT_720P50;
    stPubAttr.bDoubleFrame = HI_FALSE;
    if (HI_SUCCESS != sw_COMM_SYS_Init(&stVbConf))
    {
        printf("func:%s,line:%d\n", __FUNCTION__, __LINE__);
        return -1;
    }


    s32Ret = sw_COMM_VO_StartDevLayer(VoDev,&stPubAttr,25);
    if (HI_SUCCESS != s32Ret)
    {
        printf("%s: Start DevLayer failed!\n", __FUNCTION__);
        sw_COMM_SYS_Exit();
        return -1;
    }

    if (HI_SUCCESS != sw_COMM_VO_StartChn(VoDev, &stPubAttr, stVoMode))
    {
        printf("%s: Start VOChn failed!\n", __FUNCTION__);
        sw_COMM_SYS_Exit();
        return -1;
    }

    stInfo0.layer   =  0;
    stInfo0.fd      = -1;
    stInfo0.ctrlkey =  2;
    if (HI_TRUE == bExtendedMode)
    {
        pthread_create(&phifb0,0,sw_HIFB_REFRESH,(void *)(&stInfo0));
    }


    stInfo1.layer   =  3;
    stInfo1.fd      = -1;
    stInfo1.ctrlkey =  3;
    if (HI_SUCCESS != HI_MPI_VO_GfxLayerUnBindDev(GRAPHICS_LAYER_HC0, sw_VO_DEV_DHD0))
    {
        printf("%s: Graphic UnBind to VODev failed!,line:%d\n", __FUNCTION__, __LINE__);
        sw_COMM_SYS_Exit();
        sw_HIFB_VO_Stop();
        return -1;
    }

    if (HI_SUCCESS != HI_MPI_VO_GfxLayerBindDev(GRAPHICS_LAYER_HC0, sw_VO_DEV_DHD0))
    {
        printf("%s: Graphic Bind to VODev failed!,line:%d\n", __FUNCTION__, __LINE__);
        sw_COMM_SYS_Exit();
        sw_HIFB_VO_Stop();
        return -1;
    }
	pthread_join(phifb0,0);

    HI_MPI_SYS_Exit();
    HI_MPI_VB_Exit();
    return 0;
}

